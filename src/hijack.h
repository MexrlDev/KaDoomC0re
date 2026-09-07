#ifndef HIJACK_H
#define HIJACK_H
#include "core.h"
struct video_ctx {
    void *G, *D;
    void *vid_open, *vid_close, *vid_reg, *vid_flip, *vid_rate, *vid_evt;
    s32   handle;
    u64   eq;
    void *vmem;
    void *fbs[2];
};
int video_hijack(struct video_ctx *v, void *G, void *D, u64 eboot_base, struct ext_args *ext);
void video_cleanup(struct video_ctx *v);
void video_flip(struct video_ctx *v, int active);
#endif
