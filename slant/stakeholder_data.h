#ifndef STAKEHOLDER_DATA_H
#define STAKEHOLDER_DATA_H

// Stakeholder Perspective mode: offline starter set. Each prompt asks the
// room to argue from a specific role's viewpoint - useful for manufacturing
// the diverse perspective a homogenous room often can't produce on its own.
static const char *const OFFLINE_STAKEHOLDER[] = {
  "Argue this as a trustee who has to defend it to auditors.",
  "Argue this as the volunteer who'll actually be doing it weekly.",
  "Argue this as a beneficiary who's been \"consulted\" one too many times.",
  "Argue this as a funder who's backed three similar projects that quietly died.",
  "Argue this as the person whose job this idea might replace.",
  "Argue this as a skeptical journalist looking for the catch.",
  "Argue this as a rival organisation deciding whether to copy it.",
  "Argue this as someone joining the sector next year, with no institutional memory.",
  "Argue this as the quietest person in the room, who hasn't spoken yet.",
  "Argue this as a local authority partner worried about liability.",
  "Argue this as the person who has to explain it to their own board.",
  "Argue this as someone who's seen this exact idea fail before.",
  "Argue this as a fifteen-year-old who'll inherit the problem it's meant to fix.",
  "Argue this as the finance officer, three months before year-end.",
  "Argue this as a competitor charity with half the budget and twice the reach.",
  "Argue this as the IT person who has to actually support it.",
  "Argue this as someone who doesn't believe the problem is real.",
  "Argue this as a policymaker looking for a reason to defund the sector.",
  "Argue this as the most senior person in the room, saying nothing yet.",
  "Argue this as your own organisation, ten years from now, looking back.",
};
static const int OFFLINE_STAKEHOLDER_COUNT =
    sizeof(OFFLINE_STAKEHOLDER) / sizeof(OFFLINE_STAKEHOLDER[0]);

#endif
