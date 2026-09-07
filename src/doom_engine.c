#include "doom_engine.h"
#include "i_ps.h"
extern void D_DoomMain(void);
void doom_init(struct doom_ctx *ctx) {
    ctx->running = 1;
    doom_wad_data = ctx->wad_data;
    doom_wad_size = ctx->wad_size;
    I_Init(ctx->G, ctx->D, ctx->video, ctx->audio, ctx->input);
}
void doom_run(struct doom_ctx *ctx) {
    D_DoomMain();
}
void doom_shutdown(struct doom_ctx *ctx) {
    I_Quit();
    ctx->running = 0;
}
