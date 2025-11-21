#ifndef WM_H
#define WM_H

#include "../utils/types.h"
#include "../drivers/vga.h"

#define MAX_WINDOWS 8
#define STATUS_BAR_HEIGHT 1

typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
    char* buffer;
    size_t buffer_size;
    size_t buffer_pos;
    uint8_t cursor_x;
    uint8_t cursor_y;
    uint8_t scroll_offset;
    uint8_t fg_color;
    uint8_t bg_color;
    uint8_t active;
    const char* title;
} window_t;

void wm_init();
void wm_create_window(uint8_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, const char* title);
void wm_write_to_window(uint8_t id, const char* text);
void wm_write_char_to_window(uint8_t id, char c);
void wm_clear_window(uint8_t id);
void wm_render_all();
void wm_render_window(uint8_t id);
void wm_render_status_bar();
window_t* wm_get_window(uint8_t id);

#endif
