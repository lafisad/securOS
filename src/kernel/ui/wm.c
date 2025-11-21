#include "wm.h"
#include "../utils/string.h"

static window_t windows[MAX_WINDOWS];

void wm_init() {
    for (int i = 0; i < MAX_WINDOWS; i++) {
        windows[i].active = 0;
        windows[i].buffer = NULL;
    }
}

void wm_create_window(uint8_t id, uint8_t x, uint8_t y, uint8_t width, uint8_t height, const char* title) {
    if (id >= MAX_WINDOWS) return;
    
    windows[id].x = x;
    windows[id].y = y;
    windows[id].width = width;
    windows[id].height = height;
    windows[id].title = title;
    windows[id].cursor_x = 0;
    windows[id].cursor_y = 0;
    windows[id].scroll_offset = 0;
    windows[id].fg_color = VGA_COLOR_WHITE;
    windows[id].bg_color = VGA_COLOR_BLACK;
    windows[id].active = 1;
    
    // Allocate buffer
    windows[id].buffer_size = width * height * 2; // char + color
    windows[id].buffer_pos = 0;
}

void wm_write_char_to_window(uint8_t id, char c) {
    if (id >= MAX_WINDOWS || !windows[id].active) return;
    
    window_t* win = &windows[id];
    
    if (c == '\n') {
        win->cursor_x = 0;
        win->cursor_y++;
        if (win->cursor_y >= win->height) {
            win->scroll_offset++;
            win->cursor_y = win->height - 1;
        }
        return;
    }
    
    if (c == '\b') {
        if (win->cursor_x > 0) {
            win->cursor_x--;
            vga_put_entry_at(' ', win->fg_color | win->bg_color << 4, 
                           win->x + win->cursor_x, win->y + win->cursor_y);
        }
        return;
    }
    
    vga_put_entry_at(c, win->fg_color | win->bg_color << 4,
                    win->x + win->cursor_x, win->y + win->cursor_y);
    
    win->cursor_x++;
    if (win->cursor_x >= win->width) {
        win->cursor_x = 0;
        win->cursor_y++;
        if (win->cursor_y >= win->height) {
            win->scroll_offset++;
            win->cursor_y = win->height - 1;
        }
    }
}

void wm_write_to_window(uint8_t id, const char* text) {
    if (id >= MAX_WINDOWS || !windows[id].active) return;
    
    for (size_t i = 0; text[i] != '\0'; i++) {
        wm_write_char_to_window(id, text[i]);
    }
}

void wm_clear_window(uint8_t id) {
    if (id >= MAX_WINDOWS || !windows[id].active) return;
    
    window_t* win = &windows[id];
    
    for (size_t y = 0; y < win->height; y++) {
        for (size_t x = 0; x < win->width; x++) {
            vga_put_entry_at(' ', win->fg_color | win->bg_color << 4,
                           win->x + x, win->y + y);
        }
    }
    
    win->cursor_x = 0;
    win->cursor_y = 0;
    win->scroll_offset = 0;
}

void wm_render_window(uint8_t id) {
    if (id >= MAX_WINDOWS || !windows[id].active) return;
    
    window_t* win = &windows[id];
    
    // Draw border (simple for now)
    for (size_t x = 0; x < win->width; x++) {
        vga_put_entry_at('-', VGA_COLOR_DARK_GREY | VGA_COLOR_BLACK << 4,
                        win->x + x, win->y - 1);
    }
}

void wm_render_status_bar() {
    uint8_t y = VGA_HEIGHT - STATUS_BAR_HEIGHT;
    const char* status = " securOS | TiWM Active | F1=Help ";
    
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        if (x < strlen(status)) {
            vga_put_entry_at(status[x], VGA_COLOR_BLACK | VGA_COLOR_LIGHT_GREY << 4, x, y);
        } else {
            vga_put_entry_at(' ', VGA_COLOR_BLACK | VGA_COLOR_LIGHT_GREY << 4, x, y);
        }
    }
}

void wm_render_all() {
    for (int i = 0; i < MAX_WINDOWS; i++) {
        if (windows[i].active) {
            wm_render_window(i);
        }
    }
    wm_render_status_bar();
}

window_t* wm_get_window(uint8_t id) {
    if (id >= MAX_WINDOWS) return NULL;
    return &windows[id];
}
