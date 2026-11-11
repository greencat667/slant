#ifndef SCALING_DATA_H
#define SCALING_DATA_H

// Scaling mode: three lenses from Frances Westley's "scale deep / scale
// out / scale up" framework (McConnell Foundation, Tamarack Institute) -
// culture/relationships, replication/reach, and policy/institutions,
// respectively. Unlike Futures Thinking, all three lenses are equally
// content-rich, so each gets its own independently syncable pool rather
// than two fixed generic pools - see slant.ino's advance_scaling_step().

static const char *const OFFLINE_SCALE_DEEP[] = {
  "What would have to change in people's hearts and minds, not just their behaviour, for this to spread?",
  "Whose trust would this need to earn before it could ever grow?",
  "What belief is quietly holding this back that nobody's named?",
  "What would this look like if it changed how people saw themselves, not just what they did?",
  "Who would need to stop believing something they currently believe?",
  "What's the story people would need to tell about themselves for this to stick?",
  "What relationship needs to exist before any of this works at scale?",
  "What would this cost someone in status, and are they willing to pay it?",
  "What ritual or habit would need to change quietly, over years?",
  "Whose permission does this actually need, beneath the formal one?",
  "What would have to become normal that currently feels radical?",
  "What's the thing people would need to grieve before they could accept this?",
  "Who needs to feel ownership of this, not just approval of it?",
  "What would change about how power is held, not just how resources are shared?",
  "What's the version of this that takes a generation, not a grant cycle?",
  "What quiet cultural resistance are you underestimating?",
  "Whose story does this need to become part of?",
  "What would this look like if it changed a relationship before it changed a system?",
};
static const int OFFLINE_SCALE_DEEP_COUNT =
    sizeof(OFFLINE_SCALE_DEEP) / sizeof(OFFLINE_SCALE_DEEP[0]);

static const char *const OFFLINE_SCALE_OUT[] = {
  "What's the version of this a stranger could run without you in the room?",
  "What's the one page that would let someone else start tomorrow?",
  "What breaks first when you're not there to fix it personally?",
  "Who could run this in a place you've never been?",
  "What's the minimum viable version that still keeps the point of it?",
  "What would need to be true for ten more places to want this?",
  "What's the part that only works because of local knowledge you have?",
  "Who would need training, and how long would that actually take?",
  "What's the franchise version of this, and does it survive translation?",
  "What would you have to give up controlling for this to spread?",
  "What's the smallest unit this could be broken into and still work?",
  "Who's the first outsider who could prove this travels?",
  "What local variation would you actually allow, and where's the line?",
  "What's the thing that has to stay identical everywhere it goes?",
  "Who decides if a copy of this counts as a success?",
  "What would make this easy to say no to, that you should fix first?",
  "What's the version of this that fits in a box and ships?",
  "Who's already asked to run this somewhere else, and what did you say?",
};
static const int OFFLINE_SCALE_OUT_COUNT =
    sizeof(OFFLINE_SCALE_OUT) / sizeof(OFFLINE_SCALE_OUT[0]);

static const char *const OFFLINE_SCALE_UP[] = {
  "What would need to change in a funder's requirements for this to become the default?",
  "What law, regulation, or standard would make this unavoidable?",
  "Who has to lose an argument in a boardroom for this to become policy?",
  "What would a government department need to believe for this to become mandatory?",
  "What's the version of this a civil servant could defend in a select committee?",
  "Who benefits from the status quo enough to block this at a policy level?",
  "What existing regulation already almost requires this, unnoticed?",
  "What would this look like as a line item in next year's budget?",
  "Who's the one institution that could make this the industry standard?",
  "What would have to be measured officially for this to matter to policymakers?",
  "What's the manifesto commitment version of this?",
  "Who needs to be embarrassed publicly before this becomes policy?",
  "What would this cost the Treasury, and who's already modelled that?",
  "What existing legislation would need just one clause changed?",
  "Who's the unlikely political ally who'd actually back this?",
  "What would this look like if it were compulsory, not optional?",
  "What's the procurement rule that quietly prevents this right now?",
  "Who needs to see this work somewhere else first before they'll legislate for it?",
};
static const int OFFLINE_SCALE_UP_COUNT =
    sizeof(OFFLINE_SCALE_UP) / sizeof(OFFLINE_SCALE_UP[0]);

#endif
