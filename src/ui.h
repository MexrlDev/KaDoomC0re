#ifndef UI_H
#define UI_H
#include "core.h"
void ui_clear_fb(u32 *fb, u32 color);
void ui_draw_char_scaled(u32 *fb, int x, int y, char ch, int scale, u32 color);
void ui_draw_text_scaled(u32 *fb, const char *s, int scale, u32 color, int y_pos);
void ui_draw_str(u32 *fb, int x, int y, const char *s, u32 color);
int ui_str_len(const char *s);
#endif
