#ifndef README_DATA_H
#define README_DATA_H

// Read Me mode: a fixed set of help-text pages, cycled with PWR - no
// offline/learned/AI pool at all, since it's static explainer text, not a
// facilitation prompt. See slant.ino's advance_readme_step().
static const char *const README_PAGES[] = {
  "Slant: a facilitation card randomiser for innovation and charity work, in the spirit of Oblique Strategies.",
  "Top button changes mode, bottom button draws a card in that mode.",
  "11 modes: Oblique, Pre-Mortem, Stakeholder, Constraint, Steal, Values, Ecosystem, Decision, Futures, Scaling, Dickinson.",
  "Dickinson mode quotes the poet directly. Her lines on truth, possibility and resilience suit facilitation work - and give Slant its name (tell all the truth but tell it slant).",
  "The last menu item, Splash, shows the SLANT title screen. Press the bottom button to start again.",
  "Futures and Scaling are different: they step through a few related prompts instead of one random line.",
  "No WiFi needed. Everything works fully offline.",
};
static const int README_PAGE_COUNT =
    sizeof(README_PAGES) / sizeof(README_PAGES[0]);

#endif
