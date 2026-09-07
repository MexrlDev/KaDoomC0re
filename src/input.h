#ifndef INPUT_H
#define INPUT_H
#include "core.h"
#define DOOM_BTN_UP     0x01
#define DOOM_BTN_DOWN   0x02
#define DOOM_BTN_LEFT   0x04
#define DOOM_BTN_RIGHT  0x08
#define DOOM_BTN_SHOOT  0x10
#define DOOM_BTN_USE    0x20
#define DOOM_BTN_WEAPON 0x40
#define DOOM_BTN_MENU   0x80
#define DOOM_BTN_EXIT   0x100
struct input_ctx {
    void *G, *D;
    void *pad_init, *pad_geth, *pad_read;
    s32 pad_h;
    u32 state;
    u32 web_state;
    s32 web_fd;
    s32 web_client;
    u8 *web_page;
    u64 web_page_len;
};
int input_init(struct input_ctx *in, void *G, void *D, u64 eboot_base, struct ext_args *ext);
void input_poll(struct input_ctx *in);
u32 input_get_state(struct input_ctx *in);
void input_cleanup(struct input_ctx *in);
#endif
