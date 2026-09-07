#include "audio.h"
int audio_init(struct audio_ctx *a, void *G, void *D) {
    a->G = G; a->D = D;
    a->handle = -1;
    a->buffer = NULL;
    void *load_mod = SYM(G, D, LIBKERNEL_HANDLE, "sceKernelLoadStartModule");
    s32 aud_mod = (s32)NC(G, load_mod, (u64)"libSceAudioOut.sprx", 0,0,0,0,0);
    if (aud_mod < 0) return -1;
    a->aud_open  = SYM(G, D, aud_mod, "sceAudioOutOpen");
    a->aud_out   = SYM(G, D, aud_mod, "sceAudioOutOutput");
    a->aud_close = SYM(G, D, aud_mod, "sceAudioOutClose");
    if (!a->aud_open || !a->aud_out || !a->aud_close) return -2;
    a->handle = (s32)NC(G, a->aud_open, 0xFF, 0, 0, SAMPLES_PER_BUF, SAMPLE_RATE, AUDIO_S16_STEREO);
    if (a->handle < 0) return -3;
    a->buffer = (s16*)NC(G, SYM(G, D, LIBKERNEL_HANDLE, "malloc"), SAMPLES_PER_BUF * 2 * sizeof(s16), 0,0,0,0,0);
    if (!a->buffer) return -4;
    return 0;
}
void audio_submit(struct audio_ctx *a, const s16 *samples, int count) {
    if (!a->aud_out || a->handle < 0 || !a->buffer) return;
    int remaining = count;
    int offset = 0;
    while (remaining > 0) {
        int chunk = remaining < SAMPLES_PER_BUF ? remaining : SAMPLES_PER_BUF;
        NC(a->G, a->aud_out, (u64)a->handle, (u64)(samples + offset), 0, 0, 0, 0);
        remaining -= chunk;
        offset += chunk;
    }
}
void audio_cleanup(struct audio_ctx *a) {
    if (a->aud_close && a->handle >= 0)
        NC(a->G, a->aud_close, (u64)a->handle, 0,0,0,0,0);
    if (a->buffer) {
        void *free_fn = SYM(a->G, a->D, LIBKERNEL_HANDLE, "free");
        if (free_fn) NC(a->G, free_fn, (u64)a->buffer, 0,0,0,0,0);
        a->buffer = NULL;
    }
}
