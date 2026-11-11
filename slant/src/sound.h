#ifndef SOUND_H
#define SOUND_H

#ifdef __cplusplus
extern "C" {
#endif

// Brings up the ES8311 codec for playback only (mirrors the vendor's
// in_out codec config, since that's the only tested pattern for this
// board, but only ever opens the playback side - no mic capture here).
void sound_init(void);

// A short rising two-note chime, played once on a genuine cold boot.
void play_startup_chime(void);

#ifdef __cplusplus
}
#endif

#endif
