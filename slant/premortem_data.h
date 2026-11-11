#ifndef PREMORTEM_DATA_H
#define PREMORTEM_DATA_H

// Pre-Mortem mode: offline starter set. Original prompts aimed at charity/
// innovation-sector failure patterns specifically, not generic startup ones.
static const char *const OFFLINE_PREMORTEM[] = {
  "Imagine the funder pulls out after year one. What breaks first?",
  "The volunteers who built this leave. Does it survive?",
  "A journalist asks the one question you're hoping nobody asks.",
  "The pilot worked because one person cared this much. They move on.",
  "Success looks identical to failure for the first six months.",
  "The board asks why this isn't a grant application instead.",
  "The beneficiaries were never actually asked.",
  "It scales, and the thing that made it work stops being true.",
  "A well-meaning trustee vetoes it in the final meeting.",
  "The partner organisation quietly stops replying.",
  "Nobody owns it once the project funding ends.",
  "It works brilliantly and nobody outside the room ever hears about it.",
  "The data you need to prove it worked was never collected.",
  "It depends on goodwill that has a shelf life.",
  "The thing it was meant to replace never actually goes away.",
  "Six months in, it's technically still running and nobody's using it.",
  "It only works in the pilot area, for reasons nobody can name.",
  "The most enthusiastic person in the room isn't the one who has to deliver it.",
  "It quietly becomes someone's whole job, and that job gets cut.",
  "The risk nobody named out loud turns out to be the real one.",
};
static const int OFFLINE_PREMORTEM_COUNT =
    sizeof(OFFLINE_PREMORTEM) / sizeof(OFFLINE_PREMORTEM[0]);

#endif
