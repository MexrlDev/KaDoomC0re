#ifndef DOOM_ENGINE_H
#define DOOM_ENGINE_H
#include "core.h"
#include "hijack.h"
#include "audio.h"
#include "input.h"
struct doom_ctx {
    void *G, *D;
    struct video_ctx *video;
    struct audio_ctx *audio;
    struct input_ctx *input;
    u8 *wad_data;
    u64 wad_size;
    int running;
};
void doom_init(struct doom_ctx *ctx);
void doom_run(struct doom_ctx *ctx);
void doom_shutdown(struct doom_ctx *ctx);
#endif
