#include "core.h"
#include "hijack.h"
#include "audio.h"
#include "input.h"
#include "ftp_server.h"
#include "doom_engine.h"
__attribute__((section(".text._start")))
void _start(u64 eboot_base, u64 dlsym_addr, struct ext_args *ext) {
    void *G = (void *)(eboot_base + GADGET_OFFSET);
    void *D = (void *)dlsym_addr;
    void *load_mod  = SYM(G, D, LIBKERNEL_HANDLE, "sceKernelLoadStartModule");
    void *mmap      = SYM(G, D, LIBKERNEL_HANDLE, "mmap");
    void *kopen     = SYM(G, D, LIBKERNEL_HANDLE, "sceKernelOpen");
    void *kwrite    = SYM(G, D, LIBKERNEL_HANDLE, "sceKernelWrite");
    void *kclose    = SYM(G, D, LIBKERNEL_HANDLE, "sceKernelClose");
    void *kmkdir    = SYM(G, D, LIBKERNEL_HANDLE, "sceKernelMkdir");
    void *getdents  = SYM(G, D, LIBKERNEL_HANDLE, "sceKernelGetdents");
    void *usleep    = SYM(G, D, LIBKERNEL_HANDLE, "sceKernelUsleep");
    void *recvfrom  = SYM(G, D, LIBKERNEL_HANDLE, "recvfrom");
    void *sendto    = SYM(G, D, LIBKERNEL_HANDLE, "sendto");
    void *accept    = SYM(G, D, LIBKERNEL_HANDLE, "accept");
    void *getsockname_fn = SYM(G, D, LIBKERNEL_HANDLE, "getsockname");
    s32 log_fd = ext->log_fd;
    u8 *log_sa = ext->log_addr;
    s32 userId = (s32)ext->dbg[3];
    s32 ftp_srv = (s32)ext->dbg[4];
    s32 ftp_data_fd = (s32)ext->dbg[5];
    struct video_ctx video = {0};
    int ret = video_hijack(&video, G, D, eboot_base, ext);
    if (ret != 0) { ext->status = ret; ext->step = 1; return; }
    struct audio_ctx audio = {0};
    if (audio_init(&audio, G, D) != 0) {
        video_cleanup(&video);
        ext->status = -2; return;
    }
    struct input_ctx input = {0};
    if (input_init(&input, G, D, eboot_base, ext) != 0) {
        audio_cleanup(&audio);
        video_cleanup(&video);
        ext->status = -3; return;
    }
    u8 *wad_data = NULL;
    u64 wad_size = 0;
    ftp_serve(ftp_srv, ftp_data_fd, G, D, load_mod, mmap,
              kopen, kwrite, kclose, kmkdir,
              getdents, usleep,
              recvfrom, sendto, accept,
              getsockname_fn,
              log_fd, log_sa, userId,
              &wad_data, &wad_size);
    struct doom_ctx doom = {0};
    doom.G = G; doom.D = D;
    doom.video = &video;
    doom.audio = &audio;
    doom.input = &input;
    doom.wad_data = wad_data;
    doom.wad_size = wad_size;
    doom_init(&doom);
    doom_run(&doom);
    doom_shutdown(&doom);
    input_cleanup(&input);
    audio_cleanup(&audio);
    video_cleanup(&video);
    ext->status = 0;
}
