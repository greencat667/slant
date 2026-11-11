#ifndef DICKINSON_DATA_H
#define DICKINSON_DATA_H

// Emily Dickinson mode: her actual words, not "in the spirit of" - unlike
// Oblique Strategies, her poems are firmly public domain (she died in
// 1886), so these are real quoted lines. Curated for relevance to
// innovation/facilitation work specifically (possibility, truth-telling,
// resilience, learning from failure, purpose) rather than just her most
// famous lines regardless of theme.
//
// Offline only, deliberately - no relay_key, so "Sync Now" can never
// target this mode (see try_relay_strategy()'s null-relay_key guard in
// slant.ino). Fabricating "AI-generated Dickinson" would undermine
// the point of quoting her for real.
static const char *const OFFLINE_DICKINSON[] = {
  "Tell all the truth but tell it slant.",
  "I dwell in Possibility.",
  "The possible's slow fuse is lit by the imagination.",
  "Forever is composed of nows.",
  "Hope is the thing with feathers that perches in the soul.",
  "That it will never come again is what makes life so sweet.",
  "Success is counted sweetest by those who ne'er succeed.",
  "We turn not older with years, but newer every day.",
  "Not knowing when the dawn will come, I open every door.",
  "Truth is so rare a thing, it is delightful to tell it.",
  "If I can stop one heart from breaking, I shall not live in vain.",
  "The brain is wider than the sky.",
  "Saying nothing sometimes says the most.",
  "A wounded deer leaps highest.",
  "The soul selects her own society, then shuts the door.",
};
static const int OFFLINE_DICKINSON_COUNT =
    sizeof(OFFLINE_DICKINSON) / sizeof(OFFLINE_DICKINSON[0]);

#endif
