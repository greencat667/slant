#ifndef STRATEGIES_H
#define STRATEGIES_H

// A starter set of original lateral-thinking prompts, in the spirit of
// Brian Eno & Peter Schmidt's Oblique Strategies but NOT that deck - those
// cards are their copyrighted work. If you own the physical deck, replace
// this list with your own typed-up copy for personal use; otherwise, this
// is an original set to use and extend as-is.
//
// Used only when no relay/AI is reachable - see oblique_strategies.ino.

static const char *const OFFLINE_STRATEGIES[] = {
  "Remove the most important part and see what remains",
  "Work at half your usual speed",
  "Ask what a stranger would do here",
  "Swap the beginning and the end",
  "Do the boring part first",
  "Make it worse, on purpose, then study why",
  "Give the problem to someone who knows nothing about it",
  "Change only the smallest possible thing",
  "What would this look like upside down?",
  "Stop before it feels finished",
  "Use the tool you trust least",
  "Say it in one sentence",
  "Assume the constraint is the point",
  "Copy something you admire, badly, on purpose",
  "Work somewhere you've never worked before",
  "Ask what you're avoiding",
  "Trade elegance for speed, just this once",
  "Explain it to someone who isn't there",
  "Take away the deadline. Does it change?",
  "Follow the mistake instead of correcting it",
  "What's the cheapest version of this?",
  "Do it with your non-dominant hand",
  "Combine two things that don't belong together",
  "Start from the ending you're afraid of",
  "Repeat the last step, exactly, one more time",
  "Ask what the audience already knows",
  "Remove all the adjectives",
  "What if this were a gift, not a task?",
  "Make the invisible part visible",
  "Do less, but do it twice",
  "Borrow a rule from an unrelated field",
  "Which part is actually two problems?",
  "Assume you already failed. Now what?",
  "Give it a title before it exists",
  "What would you keep if everything else burned?",
  "Work in silence for ten minutes",
  "Describe the problem as weather",
  "Change the order nobody expects you to change",
  "Do it in the wrong room",
  "Trust the first idea you rejected",
  "What does the empty space want?",
  "Make a rule, then break only that one",
  "Ask the version of yourself from a year ago",
  "Subtract until it breaks, then add one thing back",
  "Pretend someone else already solved this. What did they do?",
  "Turn the biggest obstacle into the subject",
  "Use only what's already in the room",
  "What's true here that you haven't said aloud?",
  "Do the thing you'd be embarrassed to show anyone",
  "Leave it slightly wrong",
};

static const int OFFLINE_STRATEGIES_COUNT =
    sizeof(OFFLINE_STRATEGIES) / sizeof(OFFLINE_STRATEGIES[0]);

#endif
