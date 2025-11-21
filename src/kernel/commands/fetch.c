#include "fetch.h"
#include "../ui/wm.h"
#include "../drivers/vga.h"
#include "../utils/string.h"

void fetch_command(uint8_t window_id) {
    window_t* win = wm_get_window(window_id);
    if (!win) return;

    uint8_t old_fg = win->fg_color;

    // Colors
    uint8_t color_ascii = VGA_COLOR_LIGHT_CYAN;
    uint8_t color_title = VGA_COLOR_LIGHT_GREEN;
    uint8_t color_label = VGA_COLOR_LIGHT_BLUE;
    uint8_t color_text = VGA_COLOR_WHITE;
    uint8_t color_sep = VGA_COLOR_DARK_GREY;

    // Line 1
    win->fg_color = color_ascii;
    wm_write_to_window(window_id, "   _____   ");
    win->fg_color = color_title;
    wm_write_to_window(window_id, " root@securOS\n");

    // Line 2
    win->fg_color = color_ascii;
    wm_write_to_window(window_id, "  /     \\  ");
    win->fg_color = color_sep;
    wm_write_to_window(window_id, " ------------\n");

    // Line 3
    win->fg_color = color_ascii;
    wm_write_to_window(window_id, " |   |   | ");
    win->fg_color = color_label;
    wm_write_to_window(window_id, " os     ");
    win->fg_color = color_text;
    wm_write_to_window(window_id, "securOS v0.1\n");

    // Line 4
    win->fg_color = color_ascii;
    wm_write_to_window(window_id, " |   |   | ");
    win->fg_color = color_label;
    wm_write_to_window(window_id, " kernel ");
    win->fg_color = color_text;
    wm_write_to_window(window_id, "1.0.0-alpha\n");

    // Line 5
    win->fg_color = color_ascii;
    wm_write_to_window(window_id, " |   |   | ");
    win->fg_color = color_label;
    wm_write_to_window(window_id, " wm     ");
    win->fg_color = color_text;
    wm_write_to_window(window_id, "TiWM\n");

    // Line 6
    win->fg_color = color_ascii;
    wm_write_to_window(window_id, "  \\_____/  ");
    win->fg_color = color_label;
    wm_write_to_window(window_id, " shell  ");
    win->fg_color = color_text;
    wm_write_to_window(window_id, "securShell\n");

    // Line 7
    win->fg_color = color_ascii;
    wm_write_to_window(window_id, "           ");
    win->fg_color = color_label;
    wm_write_to_window(window_id, " memory ");
    win->fg_color = color_text;
    wm_write_to_window(window_id, "1440KB (Ramdisk)\n");

    // Color Palette
    win->fg_color = color_ascii;
    wm_write_to_window(window_id, "           ");
    
    win->fg_color = VGA_COLOR_BLACK;
    wm_write_to_window(window_id, "##");
    win->fg_color = VGA_COLOR_RED;
    wm_write_to_window(window_id, "##");
    win->fg_color = VGA_COLOR_GREEN;
    wm_write_to_window(window_id, "##");
    win->fg_color = VGA_COLOR_LIGHT_BROWN;
    wm_write_to_window(window_id, "##");
    win->fg_color = VGA_COLOR_BLUE;
    wm_write_to_window(window_id, "##");
    win->fg_color = VGA_COLOR_MAGENTA;
    wm_write_to_window(window_id, "##");
    win->fg_color = VGA_COLOR_CYAN;
    wm_write_to_window(window_id, "##");
    win->fg_color = VGA_COLOR_WHITE;
    wm_write_to_window(window_id, "##\n");

    // Restore color
    win->fg_color = old_fg;
    wm_write_to_window(window_id, "\n");
}
