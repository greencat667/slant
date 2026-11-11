#ifndef VALUES_DATA_H
#define VALUES_DATA_H

// Values/Ethics Check mode: offline starter set. About who gets hurt or
// overlooked quietly, not about operational viability (that's Pre-Mortem).
static const char *const OFFLINE_VALUES[] = {
  "Whose consent did you actually ask for?",
  "Who benefits if this quietly fails?",
  "Who carries the risk if you're wrong?",
  "Whose labour does this depend on that nobody's naming?",
  "What would this look like if the beneficiary designed it instead?",
  "Who loses status if this succeeds?",
  "What are you assuming someone else will object to on your behalf?",
  "Whose data are you using that they don't know about?",
  "What's the version of this a whistleblower would object to?",
  "Who's the least powerful person this decision touches?",
  "What would you have to hide from a beneficiary to keep this going?",
  "Whose culture does this quietly assume is the default?",
  "What happens to the people this replaces?",
  "Who's not in the room who should be?",
  "What's the harm you've decided is acceptable, and to whom?",
  "Whose trust are you spending that you didn't earn?",
  "What would this look like to someone who distrusts charities?",
  "Who's being helped in a way they didn't ask to be helped?",
  "What's the quiet compromise nobody's written down?",
  "Whose \"yes\" was actually just silence?",
};
static const int OFFLINE_VALUES_COUNT =
    sizeof(OFFLINE_VALUES) / sizeof(OFFLINE_VALUES[0]);

#endif
