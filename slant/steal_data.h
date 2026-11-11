#ifndef STEAL_DATA_H
#define STEAL_DATA_H

// Steal-From-Elsewhere mode: offline starter set. Cross-sector analogy
// prompts, naming a specific unrelated domain to borrow a solution from.
static const char *const OFFLINE_STEAL[] = {
  "How would a search-and-rescue team triage this?",
  "How would a theatre director stage this for an audience of one?",
  "How would a farmer solve this with what's already in the field?",
  "How would a hospital's emergency department queue this?",
  "How would a video game onboard a total beginner to this?",
  "How would a librarian organise this so anyone could find it?",
  "How would a midwife explain this to someone terrified?",
  "How would a football manager substitute their way out of this?",
  "How would a religious congregation sustain this for fifty years?",
  "How would a street market trader pitch this in ten seconds?",
  "How would a submarine crew handle this with no outside contact?",
  "How would a stand-up comedian test this in front of a hostile room?",
  "How would a museum curator make this matter to someone who wasn't there?",
  "How would a firefighter train someone to do this under pressure?",
  "How would a chef plate this so it looks like more than it is?",
  "How would a union organiser build trust for this from nothing?",
  "How would an air traffic controller sequence this under load?",
  "How would a potter make this work with the clay they have, not the clay they want?",
  "How would a night-shift nurse hand this over cleanly at 6am?",
  "How would a street performer make this compelling with no budget?",
};
static const int OFFLINE_STEAL_COUNT =
    sizeof(OFFLINE_STEAL) / sizeof(OFFLINE_STEAL[0]);

#endif
