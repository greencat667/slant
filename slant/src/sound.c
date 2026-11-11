// Simple runtime tone synthesis for the ES8311 codec - no embedded audio
// asset at all, just sine waves generated on the fly. Deliberately avoids
// the vendor's own embedded-PCM-file approach (an ESP-IDF-only linker
// mechanism, EMBED_FILES, that doesn't work under a plain arduino-cli
// build - this is exactly what caused room-state-sketch to drop the
// vendor's playback code entirely rather than fight that mechanism).
#include "sound.h"
#include "codec_board/codec_board.h"
#include "codec_board/codec_init.h"
#include "esp_codec_dev/include/esp_codec_dev.h"
#include "esp_heap_caps.h"
#include <math.h>
#include <stdlib.h>

#define SAMPLE_RATE 16000

static esp_codec_dev_handle_t playback = NULL;

void sound_init(void) {
  set_codec_board_type("S3_ePaper_1_54");
  codec_init_cfg_t codec_cfg = {0};
  codec_cfg.in_mode = CODEC_I2S_MODE_STD;   // matches vendor's tested in_out
  codec_cfg.out_mode = CODEC_I2S_MODE_STD;  // config for this exact board -
  codec_cfg.in_use_tdm = false;             // safer to mirror it than try
  codec_cfg.reuse_dev = false;              // an asymmetric one untested.
  init_codec(&codec_cfg);

  playback = get_playback_handle();
  if (playback) {
    esp_codec_dev_set_out_vol(playback, 70.0);
    esp_codec_dev_sample_info_t fs = {0};
    fs.sample_rate = SAMPLE_RATE;
    fs.channel = 2;
    fs.bits_per_sample = 16;
    esp_codec_dev_open(playback, &fs);
  }
}

// Synthesizes and plays one tone: `freq_hz` for `duration_ms`, with fixed
// (not duration-scaled) fade in/out and a given peak amplitude (0-1).
// Fixed fades matter: scaling fade time to the tone's own duration means a
// short tone can end up ALL fade and no sustain - e.g. a 45ms tone with
// fades of duration/2 each way has just 0ms left at full volume, which is
// why the first version of the swap tick was nearly inaudible even though
// its peak amplitude was the same as the (audible) startup chime's notes.
static void play_tone_ex(float freq_hz, int duration_ms, float fade_in_ms, float fade_out_ms, float amplitude) {
  if (!playback) return;

  int n_samples = SAMPLE_RATE * duration_ms / 1000;
  int16_t *buf = (int16_t *)heap_caps_malloc(n_samples * 2 * sizeof(int16_t), MALLOC_CAP_SPIRAM);
  if (!buf) return;

  float duration_s = duration_ms / 1000.0f;
  float fade_in_s = fade_in_ms / 1000.0f;
  float fade_out_s = fade_out_ms / 1000.0f;

  for (int i = 0; i < n_samples; i++) {
    float t = (float)i / SAMPLE_RATE;
    float envelope = 1.0f;
    if (t < fade_in_s) envelope = t / fade_in_s;
    else if (t > duration_s - fade_out_s) envelope = (duration_s - t) / fade_out_s;
    if (envelope < 0.0f) envelope = 0.0f;

    float sample = sinf(2.0f * (float)M_PI * freq_hz * t) * envelope * amplitude;
    int16_t s16 = (int16_t)(sample * 32767.0f);
    buf[i * 2] = s16;
    buf[i * 2 + 1] = s16;
  }

  esp_codec_dev_write(playback, buf, n_samples * 2 * sizeof(int16_t));
  free(buf);
}

void play_startup_chime(void) {
  play_tone_ex(659.25f, 150, 15.0f, 25.0f, 0.6f);  // E5
  play_tone_ex(880.00f, 220, 15.0f, 25.0f, 0.6f);  // A5
}
