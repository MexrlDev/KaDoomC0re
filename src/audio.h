#ifndef AUDIO_H
#define AUDIO_H
#include "core.h"
#define SAMPLE_RATE     48000
#define SAMPLES_PER_BUF 256
#define AUDIO_S16_STEREO 1
struct audio_ctx {
    void *G, *D;
    void *aud_open, *aud_out, *aud_close;
    s32 handle;
    s16 *buffer;
};
int audio_init(struct audio_ctx *a, void *G, void *D);
void audio_submit(struct audio_ctx *a, const s16 *samples, int count);
void audio_cleanup(struct audio_ctx *a);
#endif
