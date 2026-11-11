#ifndef FUTURES_DATA_H
#define FUTURES_DATA_H

// Futures Thinking mode is sequential, not a single random draw - see
// slant.ino's advance_futures_step(). Three steps, repeating:
//   1. A weak signal (drawn from this pool, or synced fresh via the relay
//      like any other mode - "signals" is just this mode's offline pool).
//   2. A scenario-forcing question building on it (fixed local pool below -
//      generic on purpose, since the person just read the signal and
//      doesn't need it restated).
//   3. A reversal prompt, arguing the opposite (fixed local pool below).

static const char *const OFFLINE_FUTURES_SIGNALS[] = {
  "Cities are quietly banning private cars from their centres.",
  "Insurers are starting to refuse cover in flood-risk areas.",
  "Four-day weeks are moving from trial to policy in several countries.",
  "Young people are choosing not to learn to drive at all.",
  "Local currencies are reappearing in struggling high streets.",
  "Community land trusts are buying out failing farms.",
  "Some countries are granting legal rights to rivers.",
  "Repair cafes are spreading faster than recycling centres.",
  "Universities are cutting entire departments overnight.",
  "Some cities are trialling a legal right to disconnect from work email.",
  "Micro-donations are replacing single large gifts in fundraising.",
  "Local councils are running their own energy companies again.",
  "AI-written grant applications are becoming common and funders can't tell.",
  "Some countries are trialling a shorter working week for care workers first.",
  "Community fridges are appearing faster than food banks can keep up.",
  "Some regions are legally requiring a \"right to repair\" for appliances.",
  "Younger donors are giving in public, on social media, not quietly.",
  "Some employers are offering paid volunteering days as a standard benefit.",
  "Rewilding projects are being run by pension funds, not just charities.",
  "Some places are trialling a universal basic income for care leavers.",
};
static const int OFFLINE_FUTURES_SIGNALS_COUNT =
    sizeof(OFFLINE_FUTURES_SIGNALS) / sizeof(OFFLINE_FUTURES_SIGNALS[0]);

static const char *const FUTURES_SCENARIO_PROMPTS[] = {
  "What does your work look like in fifteen years if this signal wins completely?",
  "If this becomes the normal state of the world, what does your organisation stop doing?",
  "Imagine this is now unremarkable and everywhere. What changed about how you work?",
  "What's the 2040 headline if this trend simply continues, uninterrupted?",
  "Who does this signal put out of work, and what do they do instead?",
  "If this is now government policy everywhere, what's obsolete about your current plan?",
};
static const int FUTURES_SCENARIO_COUNT =
    sizeof(FUTURES_SCENARIO_PROMPTS) / sizeof(FUTURES_SCENARIO_PROMPTS[0]);

static const char *const FUTURES_OPPOSITE_PROMPTS[] = {
  "Now argue the exact opposite has happened instead.",
  "What if this signal reverses completely within five years?",
  "Imagine this trend was a brief fad. What actually endured instead?",
  "What does the backlash against this look like, and who leads it?",
  "If this quietly failed everywhere, what replaced it instead?",
  "Argue the case that this signal was always a distraction from the real one.",
};
static const int FUTURES_OPPOSITE_COUNT =
    sizeof(FUTURES_OPPOSITE_PROMPTS) / sizeof(FUTURES_OPPOSITE_PROMPTS[0]);

#endif
