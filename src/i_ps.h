#ifndef I_PS_H
#define I_PS_H
#include "core.h"
#include "hijack.h"
#include "audio.h"
#include "input.h"
void I_Init(void *G, void *D, struct video_ctx *video, struct audio_ctx *audio, struct input_ctx *input);
void I_UpdateScreen(void);
void I_StartSound(void);
void I_SubmitSound(void);
void I_GetEvent(void);
void I_Quit(void);
extern struct video_ctx *doom_video;
extern struct audio_ctx *doom_audio;
extern struct input_ctx *doom_input;
extern u8 *doom_wad_data;
extern u64 doom_wad_size;
#endif
