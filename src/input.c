#include "input.h"
static u32 ds4_to_doom(u32 ds4_buttons) {
    u32 r = 0;
    if (ds4_buttons & 0x00000010) r |= DOOM_BTN_UP;
    if (ds4_buttons & 0x00000040) r |= DOOM_BTN_DOWN;
    if (ds4_buttons & 0x00000080) r |= DOOM_BTN_LEFT;
    if (ds4_buttons & 0x00000020) r |= DOOM_BTN_RIGHT;
    if (ds4_buttons & 0x00004000) r |= DOOM_BTN_SHOOT;
    if (ds4_buttons & 0x00008000) r |= DOOM_BTN_USE;
    if (ds4_buttons & 0x00001000) r |= DOOM_BTN_WEAPON;
    if (ds4_buttons & 0x00002000) r |= DOOM_BTN_MENU;
    if (ds4_buttons & 0x00000008) r |= DOOM_BTN_MENU;
    if (ds4_buttons & 0x00000800) r |= DOOM_BTN_EXIT;
    return r;
}
int input_init(struct input_ctx *in, void *G, void *D, u64 eboot_base, struct ext_args *ext) {
    in->G = G; in->D = D;
    in->pad_h = -1;
    in->state = 0;
    in->web_state = 0;
    in->web_fd = -1;
    in->web_client = -1;
    in->web_page = (u8*)ext->dbg[1];
    in->web_page_len = ext->dbg[2];
    void *load_mod = SYM(G, D, LIBKERNEL_HANDLE, "sceKernelLoadStartModule");
    s32 pad_mod = (s32)NC(G, load_mod, (u64)"libScePad.sprx", 0,0,0,0,0);
    if (pad_mod < 0) return -1;
    in->pad_init = SYM(G, D, pad_mod, "scePadInit");
    in->pad_geth = SYM(G, D, pad_mod, "scePadGetHandle");
    in->pad_read = SYM(G, D, pad_mod, "scePadRead");
    if (in->pad_init) NC(G, in->pad_init, 0,0,0,0,0,0);
    if (in->pad_geth) in->pad_h = (s32)NC(G, in->pad_geth, (u64)ext->dbg[3], 0,0,0,0,0);
    return 0;
}
void input_poll(struct input_ctx *in) {
    u32 combined = 0;
    if (in->pad_h >= 0 && in->pad_read) {
        u8 pad_buf[128] = {0};
        s32 n = (s32)NC(in->G, in->pad_read, (u64)in->pad_h, (u64)pad_buf, 1, 0, 0, 0);
        if (n > 0 && (u32)n < 0x80000000) {
            u32 raw = *(u32*)pad_buf;
            if (!(raw & 0x80000000))
                combined |= ds4_to_doom(raw & 0x001FFFFF);
        }
    }
    combined |= in->web_state;
    in->state = combined;
}
u32 input_get_state(struct input_ctx *in) {
    return in->state;
}
void input_cleanup(struct input_ctx *in) {
    if (in->web_client >= 0) {
        void *close = SYM(in->G, in->D, LIBKERNEL_HANDLE, "close");
        if (close) NC(in->G, close, (u64)in->web_client, 0,0,0,0,0);
        in->web_client = -1;
    }
    if (in->web_fd >= 0) {
        void *close = SYM(in->G, in->D, LIBKERNEL_HANDLE, "close");
        if (close) NC(in->G, close, (u64)in->web_fd, 0,0,0,0,0);
        in->web_fd = -1;
    }
}
