#ifndef ECOSYSTEM_DATA_H
#define ECOSYSTEM_DATA_H

// Ecosystem Scan mode: offline starter set. Anti-duplication thinking -
// who else in the sector is already solving a piece of this.
static const char *const OFFLINE_ECOSYSTEM[] = {
  "Who's already doing 80% of this?",
  "What existing thing could you bolt onto instead of building new?",
  "Who would be furious you didn't ask them first?",
  "What's the smallest organisation already solving a piece of this?",
  "Who tried this exact thing five years ago, and what happened to them?",
  "What's already fully funded that overlaps with this?",
  "Who has the relationship you'd need a year to build?",
  "What would a merger with a rival look like?",
  "Who's solving the adjacent problem you're ignoring?",
  "What infrastructure already exists that you're about to duplicate?",
  "Who would lend you their mailing list if you asked properly?",
  "What's the thing three other charities all quietly maintain separately?",
  "Who's the unglamorous partner nobody wants to share credit with?",
  "What did the sector already learn the hard way about this?",
  "Who has the data you're about to spend months collecting?",
  "What's the volunteer network already doing something adjacent?",
  "Who would this put out of a job, and could they run it instead?",
  "What's already been piloted and quietly shelved?",
  "Who's the funder already backing three versions of this?",
  "What would happen if you just asked the obvious person first?",
};
static const int OFFLINE_ECOSYSTEM_COUNT =
    sizeof(OFFLINE_ECOSYSTEM) / sizeof(OFFLINE_ECOSYSTEM[0]);

#endif
