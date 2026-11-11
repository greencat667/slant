// Prompt Deck - multi-mode facilitation card randomiser, battery/deep-sleep build
//
// A menu of six items, cycled with BOOT: the five modes (Oblique Strategies,
// Pre-Mortem, Stakeholder Perspective, Constraint Generator,
// Steal-From-Elsewhere - see modes.h) plus a trailing "Splash" entry.
// The current menu position is kept in RTC memory, which survives deep
// sleep. No timing-based gestures anywhere - every action is an explicit
// button press, which is what a device that's asleep between presses can
// actually measure reliably.
//
//   BOOT (any press) - cycle to the next menu item, show a quick
//                       confirmation, sleep again. No WiFi - fast and cheap.
//   PWR  (any press) - act on whatever's currently selected:
//                       - on a mode: instant offline card in that mode, no
//                         WiFi at all (the common case, has to be cheap on
//                         battery).
//
// Between presses the device is in ESP32 deep sleep (not the board's harder
// VBAT hardware-latch power-off - see README) - e-paper holds its image
// with zero power either way, so this is purely about MCU/radio current
// between button presses.
//
// Board: Waveshare ESP32-S3-ePaper-1.54 (V2)

#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <LittleFS.h>
#include <esp_sleep.h>
#include <driver/gpio.h>
#include <driver/rtc_io.h>
#include <vector>

#include "user_config.h"
#include "modes.h"

#include "src/display/epaper_driver_bsp.h"
#include "src/power/board_power_bsp.h"
#include "src/sound.h"

#define TEXT_MARGIN 10
#define TAG_RESERVE_PX 16  // bottom strip reserved for the source tag

struct FontChoice {
  const GFXfont *font;
};
// Tried largest-first; the biggest one whose wrapped, centered layout fits
// the panel wins - so short phrases render big, long ones shrink to fit.
static const FontChoice FONT_CANDIDATES[] = {
  {&FreeSansBold12pt7b},
  {&FreeSansBold9pt7b},
};
#define FONT_CANDIDATES_COUNT (sizeof(FONT_CANDIDATES) / sizeof(FONT_CANDIDATES[0]))

// Menu has MODE_COUNT real modes (indices 0..MODE_COUNT-1) plus one
// trailing virtual "Splash" entry (index MODE_COUNT). Both survive deep
// sleep via RTC memory, so the selection is remembered without a flash write.
RTC_DATA_ATTR static int menu_index = 0;
RTC_DATA_ATTR static int last_real_mode_index = 0;  // which mode "Sync Now" acts on
RTC_DATA_ATTR static int futures_step = 0;          // 0=signal, 1=scenario, 2=opposite
RTC_DATA_ATTR static int scaling_lens = 0;          // 0=deep, 1=out, 2=up
RTC_DATA_ATTR static int readme_step = 0;           // which help page is next

static board_power_bsp_t board_power(EPD_PWR_PIN, Audio_PWR_PIN, VBAT_PWR_PIN);
static epaper_driver_display *driver = nullptr;
static GFXcanvas1 *canvas = nullptr;

static std::vector<String> learned_strategies;  // active mode's learned pool only
static String last_offline_text;

// Second trailing virtual entry (index MODE_COUNT): re-shows the splash
// screen so the device can be handed over as a gift. Not a mode - PWR from
// here returns to the last real mode and draws a card as normal.
static bool on_splash_entry() {
  return menu_index == MODE_COUNT;
}

// The mode a card/sync action applies to - itself when the menu is sitting
// on a real mode, or whichever mode was active before landing on "Sync Now".
// This is the TOP-LEVEL mode (for menu display) - for content operations
// (offline pool, learned pool, relay key), use active_pool() instead, which
// resolves to the current lens when the mode is Scaling.
static const ModeInfo &active_mode() {
  return MODES[on_splash_entry() ? last_real_mode_index : menu_index];
}

// The actual pool content operations should use - same as active_mode()
// except for Scaling, which isn't itself a pool: it redirects to whichever
// of its three lenses (deep/out/up) is currently selected.
static const ModeInfo &active_pool() {
  const ModeInfo &m = active_mode();
  if (m.scaling_lenses) return m.scaling_lenses[scaling_lens];
  return m;
}

static bool strategy_known(const String &s) {
  const ModeInfo &m = active_pool();
  for (int i = 0; i < m.offline_count; i++) {
    if (s.equalsIgnoreCase(m.offline[i])) return true;
  }
  for (auto &ls : learned_strategies) {
    if (s.equalsIgnoreCase(ls)) return true;
  }
  return false;
}

static void save_learned() {
  if (!active_pool().learned_file) return;  // Read Me has no pool at all
  File f = LittleFS.open(active_pool().learned_file, "w");
  if (!f) {
    Serial.println("[fs] failed to open learned file for write");
    return;
  }
  for (auto &ls : learned_strategies) f.println(ls);
  f.close();
}

// Loads ONLY the active mode's learned pool - each mode's file is small,
// and only one mode is ever active at a time, so there's no need to hold
// all five in RAM at once.
static void load_learned() {
  learned_strategies.clear();
  if (!active_pool().learned_file) return;  // Read Me has no pool at all
  if (!LittleFS.begin(true)) {  // true = format on first-ever mount
    Serial.println("[fs] LittleFS mount failed");
    return;
  }
  File f = LittleFS.open(active_pool().learned_file, "r");
  if (!f) return;  // nothing learned yet for this mode - fine
  while (f.available()) {
    String line = f.readStringUntil('\n');
    line.trim();
    if (line.length() > 0) learned_strategies.push_back(line);
  }
  f.close();
  Serial.printf("[fs] loaded %d learned (%s)\n", (int)learned_strategies.size(), active_pool().name);
}

static String pick_offline_strategy() {
  const ModeInfo &m = active_pool();
  int total = m.offline_count + (int)learned_strategies.size();
  if (total <= 0) return String("(no prompts in this pool)");  // shouldn't happen for a real mode
  String choice;
  int guard = 0;
  do {
    int idx = esp_random() % total;
    choice = (idx < m.offline_count) ? String(m.offline[idx]) : learned_strategies[idx - m.offline_count];
    guard++;
  } while (choice == last_offline_text && total > 1 && guard < 10);
  last_offline_text = choice;
  return choice;
}

// Plain random pick from a small fixed local pool (no learned/AI pool of its
// own) - used for Futures Thinking's scenario/opposite follow-up steps,
// which are generic framing prompts, not content specific to any one signal.
static String pick_from_pool(const char *const *pool, int count) {
  return String(pool[esp_random() % count]);
}

// Futures Thinking is sequential, not a single random draw: step 0 shows a
// signal (from this mode's own offline/learned/synced pool, same mechanism
// as every other mode), step 1 asks a scenario question built on it, step 2
// asks for the reversal, then it loops back to a fresh signal. Returns the
// tag to show alongside the text.
static const char *advance_futures_step(String &out) {
  const char *tag;
  if (futures_step == 0) {
    out = pick_offline_strategy();
    tag = "(signal)";
  } else if (futures_step == 1) {
    out = pick_from_pool(FUTURES_SCENARIO_PROMPTS, FUTURES_SCENARIO_COUNT);
    tag = "(2/3)";
  } else {
    out = pick_from_pool(FUTURES_OPPOSITE_PROMPTS, FUTURES_OPPOSITE_COUNT);
    tag = "(3/3)";
  }
  futures_step = (futures_step + 1) % 3;
  return tag;
}

// Scaling cycles through three lenses (Westley's scale deep / out / up),
// each with its own independently syncable pool - unlike Futures, all three
// lenses are equally content-rich, so there's no single "AI-worthy" step
// and two fixed ones; pick_offline_strategy() already resolves to whichever
// lens active_pool() currently points at.
static const char *advance_scaling_step(String &out) {
  out = pick_offline_strategy();
  static const char *TAGS[] = {"(scale deep)", "(scale out)", "(scale up)"};
  const char *tag = TAGS[scaling_lens];
  scaling_lens = (scaling_lens + 1) % SCALING_LENS_COUNT;
  return tag;
}

// Read Me pages through a fixed set of static help text - no offline/
// learned/AI pool at all, since it's not a facilitation prompt.
static const char *advance_readme_step(String &out) {
  static char tag_buf[8];
  out = String(README_PAGES[readme_step]);
  snprintf(tag_buf, sizeof(tag_buf), "(%d/%d)", readme_step + 1, README_PAGE_COUNT);
  readme_step = (readme_step + 1) % README_PAGE_COUNT;
  return tag_buf;
}

static std::vector<String> wrap_text_lines(const String &text, int max_width) {
  std::vector<String> lines;
  String line;
  int start = 0;
  int len = text.length();
  while (start < len) {
    int space_idx = text.indexOf(' ', start);
    String word = (space_idx == -1) ? text.substring(start) : text.substring(start, space_idx);
    String candidate = line.length() ? (line + " " + word) : word;

    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(candidate, 0, 0, &x1, &y1, &w, &h);

    if (w > (uint16_t)max_width && line.length()) {
      lines.push_back(line);
      line = word;
    } else {
      line = candidate;
    }
    start = (space_idx == -1) ? len : space_idx + 1;
  }
  if (line.length()) lines.push_back(line);
  return lines;
}

// Wraps `text` at `font`'s metrics and reports whether the resulting block
// fits in max_height. font->yAdvance is that font's natural line-to-line
// spacing, so line height comes from the font itself rather than a guess.
static bool layout_with_font(const String &text, const GFXfont *font, int max_width, int max_height,
                              std::vector<String> &out_lines, int &out_line_height) {
  canvas->setFont(font);
  out_lines = wrap_text_lines(text, max_width);
  out_line_height = font->yAdvance;
  int total_height = out_line_height * (int)out_lines.size();
  return total_height <= max_height;
}

// Pushes whatever's currently drawn on `canvas` to the physical panel.
// Canvas: bit 1 = drawn (foreground), bit 0 = background. Driver: bit 1 =
// white, bit 0 = black. So a drawn pixel -> black, background -> white.
static void commit_canvas() {
  for (int y = 0; y < EPD_HEIGHT; y++) {
    for (int x = 0; x < EPD_WIDTH; x++) {
      bool drawn = canvas->getPixel(x, y);
      driver->EPD_DrawColorPixel((uint16_t)x, (uint16_t)y, drawn ? DRIVER_COLOR_BLACK : DRIVER_COLOR_WHITE);
    }
  }
  driver->EPD_Display();
}

// `tag` is shown bottom-right, e.g. "(ai)", "(2/3)" - or "" for the plain
// offline case, which is the common one and doesn't need calling out.
static void render_card(const String &text, const char *tag) {
  canvas->fillScreen(0);
  canvas->setTextColor(1);
  canvas->setTextWrap(false);  // we do our own word-wrap

  int max_width = EPD_WIDTH - 2 * TEXT_MARGIN;
  int max_height = EPD_HEIGHT - 2 * TEXT_MARGIN - TAG_RESERVE_PX;

  std::vector<String> lines;
  int line_height = 0;
  bool fitted = false;
  for (size_t i = 0; i < FONT_CANDIDATES_COUNT; i++) {
    if (layout_with_font(text, FONT_CANDIDATES[i].font, max_width, max_height, lines, line_height)) {
      fitted = true;
      break;
    }
  }
  if (!fitted) {
    // Best effort with the smallest candidate - may run slightly tight for
    // an unusually long phrase, but still legible rather than blocked.
    layout_with_font(text, FONT_CANDIDATES[FONT_CANDIDATES_COUNT - 1].font, max_width, max_height, lines, line_height);
  }

  int total_height = line_height * (int)lines.size();
  int extra_space = max_height - total_height;
  int top_y = TEXT_MARGIN + (extra_space > 0 ? extra_space / 2 : 0);

  int cursor_top = top_y;
  for (auto &ln : lines) {
    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(ln, 0, 0, &x1, &y1, &w, &h);
    int cx = (EPD_WIDTH - (int)w) / 2 - x1;   // horizontally centered
    int cy = cursor_top - y1;                 // baseline for this line's top
    canvas->setCursor(cx, cy);
    canvas->print(ln);
    cursor_top += line_height;
  }

  // Small tag, bottom-right, using the classic built-in font (its cursor is
  // top-left based, unlike the custom fonts above).
  canvas->setFont(nullptr);
  canvas->setTextSize(1);
  int16_t x1, y1;
  uint16_t w, h;
  canvas->getTextBounds(tag, 0, 0, &x1, &y1, &w, &h);
  canvas->setCursor(EPD_WIDTH - w - 4, EPD_HEIGHT - h - 4);
  canvas->print(tag);

  commit_canvas();
}

// The BOOT mode-cycle confirmation: an icon (ICON_SIZE x ICON_SIZE, from mode_icons.h)
// centered near the top, the mode name wrapped/centered below it. `icon`
// may be null (falls back to name-only, centered in the full height).
static void render_menu_item(const uint8_t *icon, const String &name) {
  canvas->fillScreen(0);
  canvas->setTextColor(1);
  canvas->setTextWrap(false);

  int max_width = EPD_WIDTH - 2 * TEXT_MARGIN;
  int text_top = TEXT_MARGIN;
  int text_max_height = EPD_HEIGHT - 2 * TEXT_MARGIN;

  if (icon) {
    int icon_x = (EPD_WIDTH - ICON_SIZE) / 2;
    int icon_y = 14;
    canvas->drawBitmap(icon_x, icon_y, icon, ICON_SIZE, ICON_SIZE, 1);
    text_top = icon_y + ICON_SIZE + 10;
    text_max_height = EPD_HEIGHT - text_top - TEXT_MARGIN;
  }

  std::vector<String> lines;
  int line_height = 0;
  bool fitted = false;
  for (size_t i = 0; i < FONT_CANDIDATES_COUNT; i++) {
    if (layout_with_font(name, FONT_CANDIDATES[i].font, max_width, text_max_height, lines, line_height)) {
      fitted = true;
      break;
    }
  }
  if (!fitted) {
    layout_with_font(name, FONT_CANDIDATES[FONT_CANDIDATES_COUNT - 1].font, max_width, text_max_height, lines, line_height);
  }

  int total_height = line_height * (int)lines.size();
  int extra_space = text_max_height - total_height;
  int cursor_top = text_top + (extra_space > 0 ? extra_space / 2 : 0);

  for (auto &ln : lines) {
    int16_t x1, y1;
    uint16_t w, h;
    canvas->getTextBounds(ln, 0, 0, &x1, &y1, &w, &h);
    int cx = (EPD_WIDTH - (int)w) / 2 - x1;
    int cy = cursor_top - y1;
    canvas->setCursor(cx, cy);
    canvas->print(ln);
    cursor_top += line_height;
  }

  commit_canvas();
}

// Shown once, only on a genuine cold boot (fresh flash, power-on, or a
// crash recovery - not a normal deep-sleep wake). A dedicated big title
// screen rather than routing through render_card/render_menu_item, since
// it's not content or a menu confirmation - just a one-time hello. The
// first real button press afterward behaves completely normally.
static void render_splash() {
  canvas->fillScreen(0);
  canvas->setTextColor(1);
  canvas->setTextWrap(false);

  int16_t x1, y1;
  uint16_t w, h;

  // Small lead-in line, classic built-in font - same register as the tags
  // and hint text used elsewhere. Cursor y is top-of-text for this font
  // (unlike the custom fonts below), so no y1 offset needed.
  canvas->setFont(nullptr);
  canvas->setTextSize(1);
  const char *lead_in = "Tell all the truth but tell it";
  canvas->getTextBounds(lead_in, 0, 0, &x1, &y1, &w, &h);
  int lead_in_top = 32;
  canvas->setCursor((EPD_WIDTH - (int)w) / 2 - x1, lead_in_top);
  canvas->print(lead_in);
  int lead_in_bottom = lead_in_top + (int)h;

  // The big reveal - "SLANT" completes the Dickinson line without repeating
  // it. Custom font cursor is baseline-based, hence the y1 offset.
  canvas->setFont(&FreeSansBold24pt7b);
  const char *title = "SLANT";
  canvas->getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
  int title_top = lead_in_bottom + 20;
  canvas->setCursor((EPD_WIDTH - (int)w) / 2 - x1, title_top - y1);
  canvas->print(title);

  canvas->setFont(nullptr);
  canvas->setTextSize(1);
  const char *subtitle = "press bottom button to begin";
  canvas->getTextBounds(subtitle, 0, 0, &x1, &y1, &w, &h);
  canvas->setCursor((EPD_WIDTH - (int)w) / 2 - x1, EPD_HEIGHT - 20);
  canvas->print(subtitle);

  commit_canvas();
}

static void go_to_deep_sleep() {
  board_power.POWEER_EPD_OFF();  // panel keeps its image with zero power anyway
  board_power.POWEER_Audio_OFF();

  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
  const uint64_t wake_mask = (1ULL << PWR_BUTTON_PIN) | (1ULL << BOOT_BUTTON_PIN);
  esp_sleep_enable_ext1_wakeup_io(wake_mask, ESP_EXT1_WAKEUP_ANY_LOW);
  rtc_gpio_pullup_en((gpio_num_t)PWR_BUTTON_PIN);
  rtc_gpio_pulldown_dis((gpio_num_t)PWR_BUTTON_PIN);
  rtc_gpio_pullup_en((gpio_num_t)BOOT_BUTTON_PIN);
  rtc_gpio_pulldown_dis((gpio_num_t)BOOT_BUTTON_PIN);

  // The VBAT rail's GPIO state isn't retained through deep sleep unless
  // explicitly held - see Waveshare's own 11_RTC_Sleep_Test example, which
  // this follows. Released again on the next wake, before setup() re-drives it.
  rtc_gpio_hold_en((gpio_num_t)VBAT_PWR_PIN);

  Serial.println("Going to sleep until PWR or BOOT is pressed...");
  Serial.flush();
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  delay(300);

  // Release the VBAT hold from the previous sleep before driving that pin
  // again - a no-op on the very first boot, when nothing was ever held.
  gpio_hold_dis((gpio_num_t)VBAT_PWR_PIN);
  board_power.VBAT_POWER_ON();

  pinMode(PWR_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);

  // Which pin (if either) woke us, so BOOT and PWR can mean different things.
  // A wake NOT caused by either button (fresh flash, power-on, brownout,
  // crash recovery) is a cold boot - shown once as a splash, handled
  // separately below rather than folded into the PWR-press path.
  bool woke_ext1 = (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT1);
  uint64_t wake_pins = woke_ext1 ? esp_sleep_get_ext1_wakeup_status() : 0;
  bool boot_woke = wake_pins & (1ULL << BOOT_BUTTON_PIN);
  bool pwr_woke = wake_pins & (1ULL << PWR_BUTTON_PIN);
  bool cold_boot = !woke_ext1;

  board_power.POWEER_EPD_ON();

  custom_lcd_spi_t epd_cfg = {};
  epd_cfg.cs = EPD_CS_PIN;
  epd_cfg.dc = EPD_DC_PIN;
  epd_cfg.rst = EPD_RST_PIN;
  epd_cfg.busy = EPD_BUSY_PIN;
  epd_cfg.mosi = EPD_MOSI_PIN;
  epd_cfg.scl = EPD_SCK_PIN;
  epd_cfg.spi_host = EPD_SPI_NUM;
  epd_cfg.buffer_len = 5000;

  driver = new epaper_driver_display(EPD_WIDTH, EPD_HEIGHT, epd_cfg);
  driver->EPD_Init();
  driver->EPD_Clear();
  driver->EPD_Display();

  canvas = new GFXcanvas1(EPD_WIDTH, EPD_HEIGHT);

  if (cold_boot) {
    Serial.println("[boot] cold boot - showing splash");
    // Audio only ever needs to come up for the cold-boot chime - the
    // per-card tick was tried and dropped (didn't add anything over the
    // e-paper's own refresh flash - see README), so regular presses stay
    // audio-free and fast, same as before that experiment.
    board_power.POWEER_Audio_ON();
    sound_init();
    render_splash();
    play_startup_chime();
  } else if (boot_woke) {
    // Cycle the menu (modes, then the trailing "Splash" entry) - no WiFi,
    // just a confirmation of the new selection.
    menu_index = (menu_index + 1) % (MODE_COUNT + 1);
    if (on_splash_entry()) {
      Serial.println("[menu] Splash");
      render_splash();
    } else if (MODES[menu_index].readme_pages) {
      // Read Me always starts fresh at page 1 on entry - unlike
      // Futures/Scaling, it's a manual to browse, not a sequence you're
      // meant to resume mid-way through. It also has no pool for "Sync Now"
      // to target, so last_real_mode_index is deliberately left alone here,
      // pointing at whatever real mode preceded it.
      readme_step = 0;
      Serial.printf("[menu] switched to %s\n", active_mode().name);
      render_menu_item(active_mode().icon, active_mode().name);
    } else {
      last_real_mode_index = menu_index;
      Serial.printf("[menu] switched to %s\n", active_mode().name);
      render_menu_item(active_mode().icon, active_mode().name);
    }
  } else {
    // PWR on the Splash entry = "begin": back to the last real mode.
    if (on_splash_entry()) menu_index = last_real_mode_index;
    load_learned();

    // Captured BEFORE dispatch: advance_scaling_step() mutates scaling_lens,
    // which active_pool() itself depends on for Scaling - evaluating this
    // after dispatch would log the NEXT lens, not the one actually drawn
    // from (the tag and rendered text are unaffected either way, since
    // those are captured correctly inside each advance_*_step() function;
    // only this diagnostic log line was at risk of the stale read).
    String pool_name = active_pool().name;

    String text;
    const char *tag;
    if (active_mode().readme_pages) {
      tag = advance_readme_step(text);
    } else if (active_mode().sequential) {
      tag = advance_futures_step(text);
    } else if (active_mode().scaling_lenses) {
      tag = advance_scaling_step(text);
    } else {
      text = pick_offline_strategy();
      tag = "";
    }
    Serial.printf("[card] (%s, %s) %s\n", tag[0] ? tag : "offline", pool_name.c_str(), text.c_str());
    render_card(text, tag);
  }

  go_to_deep_sleep();
}

void loop() {}  // never reached - setup() always ends in deep sleep
