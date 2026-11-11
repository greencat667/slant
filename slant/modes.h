#ifndef MODES_H
#define MODES_H

#include "strategies.h"
#include "premortem_data.h"
#include "stakeholder_data.h"
#include "constraint_data.h"
#include "steal_data.h"
#include "values_data.h"
#include "ecosystem_data.h"
#include "decision_data.h"
#include "futures_data.h"
#include "scaling_data.h"
#include "readme_data.h"
#include "dickinson_data.h"
#include "mode_icons.h"

struct ModeInfo {
  const char *name;             // shown on the mode-cycle confirmation card
  const char *relay_key;        // sent to the relay as ?mode=<relay_key>
  const char *const *offline;   // this mode's offline starter list
  int offline_count;
  const char *learned_file;     // this mode's own LittleFS learned-pool path
  bool sequential;               // true only for Futures Thinking - see
                                 // slant.ino's advance_futures_step()

  // Set only for Scaling: a pointer to its 3 lenses (each itself a
  // ModeInfo, reused as a "leaf pool" - name/relay_key/offline/learned_file
  // per lens, scaling_lenses left null on the lenses themselves so they
  // don't nest). See slant.ino's active_pool() and
  // advance_scaling_step(). Null for every other mode.
  const struct ModeInfo *scaling_lenses;
  int scaling_lens_count;

  // Set only for Read Me: a fixed set of static help-text pages, cycled
  // with no offline/learned/AI pool at all - see slant.ino's
  // advance_readme_step(). Null for every other mode.
  const char *const *readme_pages;
  int readme_page_count;

  const uint8_t *icon;  // 80x80 1bpp PROGMEM bitmap, shown on the BOOT
                        // mode-cycle confirmation - see mode_icons.h
};

// Scaling's three lenses (Frances Westley's scale deep / out / up
// framework) - not top-level menu entries themselves, just the pools
// Scaling's own menu entry cycles through.
static const ModeInfo SCALING_LENSES[] = {
  {"Scale Deep", "scaling_deep", OFFLINE_SCALE_DEEP, OFFLINE_SCALE_DEEP_COUNT, "/learned_scale_deep.txt", false, nullptr, 0, nullptr, 0, nullptr},
  {"Scale Out",  "scaling_out",  OFFLINE_SCALE_OUT,  OFFLINE_SCALE_OUT_COUNT,  "/learned_scale_out.txt",  false, nullptr, 0, nullptr, 0, nullptr},
  {"Scale Up",   "scaling_up",   OFFLINE_SCALE_UP,   OFFLINE_SCALE_UP_COUNT,   "/learned_scale_up.txt",   false, nullptr, 0, nullptr, 0, nullptr},
};
static const int SCALING_LENS_COUNT = sizeof(SCALING_LENSES) / sizeof(SCALING_LENSES[0]);

static const ModeInfo MODES[] = {
  {"Read Me",                  nullptr,       nullptr,             0,       nullptr, false, nullptr, 0, README_PAGES, README_PAGE_COUNT, ICON_READ_ME},
  {"Oblique Strategies",       "oblique",     OFFLINE_STRATEGIES,  OFFLINE_STRATEGIES_COUNT,  "/learned_oblique.txt", false, nullptr, 0, nullptr, 0, ICON_OBLIQUE},
  {"Pre-Mortem",               "premortem",   OFFLINE_PREMORTEM,   OFFLINE_PREMORTEM_COUNT,   "/learned_premortem.txt", false, nullptr, 0, nullptr, 0, ICON_PREMORTEM},
  {"Stakeholder Perspective",  "stakeholder", OFFLINE_STAKEHOLDER, OFFLINE_STAKEHOLDER_COUNT, "/learned_stakeholder.txt", false, nullptr, 0, nullptr, 0, ICON_STAKEHOLDER},
  {"Constraint Generator",     "constraint",  OFFLINE_CONSTRAINT,  OFFLINE_CONSTRAINT_COUNT,  "/learned_constraint.txt", false, nullptr, 0, nullptr, 0, ICON_CONSTRAINT},
  {"Steal From Elsewhere",     "steal",       OFFLINE_STEAL,       OFFLINE_STEAL_COUNT,       "/learned_steal.txt", false, nullptr, 0, nullptr, 0, ICON_STEAL},
  {"Values Check",             "values",      OFFLINE_VALUES,      OFFLINE_VALUES_COUNT,      "/learned_values.txt", false, nullptr, 0, nullptr, 0, ICON_VALUES},
  {"Ecosystem Scan",           "ecosystem",   OFFLINE_ECOSYSTEM,   OFFLINE_ECOSYSTEM_COUNT,   "/learned_ecosystem.txt", false, nullptr, 0, nullptr, 0, ICON_ECOSYSTEM},
  {"Decision Forcer",          "decision",    OFFLINE_DECISION,    OFFLINE_DECISION_COUNT,    "/learned_decision.txt", false, nullptr, 0, nullptr, 0, ICON_DECISION},
  {"Futures Thinking",         "futures",     OFFLINE_FUTURES_SIGNALS, OFFLINE_FUTURES_SIGNALS_COUNT, "/learned_futures.txt", true, nullptr, 0, nullptr, 0, ICON_FUTURES},
  {"Scaling",                  nullptr,       nullptr,             0,                          nullptr, false, SCALING_LENSES, SCALING_LENS_COUNT, nullptr, 0, ICON_SCALING},
  // No relay_key - offline only, deliberately. try_relay_strategy() already
  // guards on a null relay_key, so "Sync Now" gracefully no-ops (falls back
  // to an offline pick) rather than ever fabricating "AI-generated Dickinson".
  {"Emily Dickinson",          nullptr,       OFFLINE_DICKINSON,   OFFLINE_DICKINSON_COUNT,   "/learned_dickinson.txt", false, nullptr, 0, nullptr, 0, ICON_DICKINSON},
};
static const int MODE_COUNT = sizeof(MODES) / sizeof(MODES[0]);

#endif
