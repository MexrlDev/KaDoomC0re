#include "i_ps.h"
#include "ui.h"
#include <string.h>
struct video_ctx *doom_video = NULL;
struct audio_ctx *doom_audio = NULL;
struct input_ctx *doom_input = NULL;
u8 *doom_wad_data = NULL;
u64 doom_wad_size = 0;
static u8 *doom_screen = NULL;
static int screen_w = 320, screen_h = 200;

// ----- stubs for functions called by the engine -----
void I_Error(const char *error, ...) { while (1) {} }
void I_GetTime(void) { /* dummy */ }
unsigned int I_GetTimeMS(void) { static unsigned int ms = 0; return ms++; }
void I_Sleep(unsigned int ms) {
    if (doom_video && doom_video->G) {
        void *usleep = SYM(doom_video->G, doom_video->D, LIBKERNEL_HANDLE, "sceKernelUsleep");
        if (usleep) NC(doom_video->G, usleep, ms * 1000ULL, 0,0,0,0,0);
    }
}
void I_AtExit(void (*func)(void)) {}
void I_StartTic(void) {}
void I_ShutdownGraphics(void) {}
void I_GetEvent(void) { input_poll(doom_input); }
// ----- end of stubs -----

void I_Init(void *G, void *D, struct video_ctx *video, struct audio_ctx *audio, struct input_ctx *input) {
    doom_video = video;
    doom_audio = audio;
    doom_input = input;
    doom_screen = (u8*)NC(G, SYM(G, D, LIBKERNEL_HANDLE, "malloc"), screen_w * screen_h, 0,0,0,0,0);
    memset(doom_screen, 0, screen_w * screen_h);
}
void I_UpdateScreen(void) {
    if (!doom_video || !doom_screen) return;
    u32 *fb = (u32*)doom_video->fbs[doom_video->handle % 2];
    ui_clear_fb(fb, 0xFF000000);
    int scale = 6;
    int off_x = (SCR_W - screen_w * scale) / 2;
    int off_y = (SCR_H - screen_h * scale) / 2;
    for (int y = 0; y < screen_h; y++) {
        for (int x = 0; x < screen_w; x++) {
            u8 pixel = doom_screen[y * screen_w + x];
            u32 color = 0xFF000000 | (pixel << 16) | (pixel << 8) | pixel;
            for (int dy = 0; dy < scale; dy++) {
                for (int dx = 0; dx < scale; dx++) {
                    int px = off_x + x * scale + dx;
                    int py = off_y + y * scale + dy;
                    if (px < SCR_W && py < SCR_H)
                        fb[py * SCR_W + px] = color;
                }
            }
        }
    }
    video_flip(doom_video, doom_video->handle % 2);
    doom_video->handle++;
}
void I_StartSound(void) {}
void I_SubmitSound(void) {}
void I_Quit(void) {
    if (doom_screen) {
        void *free_fn = SYM(doom_video->G, doom_video->D, LIBKERNEL_HANDLE, "free");
        if (free_fn) NC(doom_video->G, free_fn, (u64)doom_screen, 0,0,0,0,0);
        doom_screen = NULL;
    }
}
