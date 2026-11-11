#ifndef CONSTRAINT_DATA_H
#define CONSTRAINT_DATA_H

// Constraint Generator mode: offline starter set. Artificial constraints
// for time-boxed sprints - forces resource-limited, concrete thinking.
static const char *const OFFLINE_CONSTRAINT[] = {
  "You have 50 pounds and two days.",
  "No digital tools of any kind are allowed.",
  "It has to work for someone who's never used a smartphone.",
  "You can only use what's already in the building.",
  "It has to be explainable in one sentence to a stranger.",
  "Nobody involved is allowed to be paid for their time.",
  "It must work without an internet connection.",
  "You have to hand it off to someone else halfway through.",
  "It can't require anyone to change their existing habits.",
  "The whole thing has to fit on a single sheet of paper.",
  "You're not allowed to build anything new - only rearrange what exists.",
  "It has to survive being run by volunteers with two hours a month.",
  "Assume no one will read an email about it.",
  "It must cost less to run than it costs to explain.",
  "You have one week, then it's judged in public.",
  "It has to work exactly as well in the least-resourced location as the best.",
  "Nobody is allowed to use the word \"platform.\"",
  "It must be reversible within a day if it doesn't work.",
  "You can only recruit people who've said no to something like this before.",
  "Assume the funding disappears the moment it starts working.",
};
static const int OFFLINE_CONSTRAINT_COUNT =
    sizeof(OFFLINE_CONSTRAINT) / sizeof(OFFLINE_CONSTRAINT[0]);

#endif
