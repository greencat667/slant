#ifndef DECISION_DATA_H
#define DECISION_DATA_H

// Decision Forcer mode: offline starter set. Forces a concrete commitment
// or kill decision in the moment - the device can't track follow-through
// over time (no input capability), so this is the in-the-room version of
// that idea rather than a persistent commitment tracker.
static const char *const OFFLINE_DECISION[] = {
  "Which of the three ideas on the table do you kill before this meeting ends?",
  "Name the thing you're keeping alive out of guilt, not merit.",
  "What are you doing that you'd cut immediately if someone else proposed it?",
  "What decision have you been avoiding for a month?",
  "Which stakeholder's objection are you quietly waiting out?",
  "What's the thing everyone agrees should stop, that hasn't stopped?",
  "Pick one thing to stop funding today, out loud, right now.",
  "What would you commit to if you had to announce it publicly tomorrow?",
  "Which meeting on your calendar should simply not happen again?",
  "What's the version of this you'd choose if you couldn't ask for more time?",
  "Name the person you need to disappoint to make progress.",
  "What's the thing you're still doing \"for now\" that's been true for a year?",
  "Which of your current projects would you defend hardest, and why does that worry you?",
  "What's the smallest version of this you could commit to by Friday?",
  "Whose sign-off are you waiting for that you don't actually need?",
  "What decision are you outsourcing to \"the data\" to avoid making it yourself?",
  "Which relationship are you maintaining past its usefulness?",
  "What's the thing you'll regret not stopping sooner?",
  "Commit to one number you'll be judged against in six months.",
  "What are you willing to be wrong about, in public, today?",
};
static const int OFFLINE_DECISION_COUNT =
    sizeof(OFFLINE_DECISION) / sizeof(OFFLINE_DECISION[0]);

#endif
