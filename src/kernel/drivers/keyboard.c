#include "keyboard.h"
#include "keyboard.h"
#include "ports.h"
#include "vga.h"

static const char scancode_to_ascii_qwerty[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

static const char scancode_to_ascii_shift_qwerty[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' '
};

static const char scancode_to_ascii_qwertz[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'z', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'y', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0,
    '*', 0, ' '
};

static const char scancode_to_ascii_shift_qwertz[] = {
    0, 0, '!', '"', 0, '$', '%', '&', '/', '(', ')', '=', '?', '`', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Z', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\'', '>',
    0, '|', 'Y', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 0,
    '*', 0, ' '
};

static const char* current_layout_normal = scancode_to_ascii_qwerty;
static const char* current_layout_shift = scancode_to_ascii_shift_qwerty;
static int shift_pressed = 0;

void keyboard_init() {
    shift_pressed = 0;
    current_layout_normal = scancode_to_ascii_qwerty;
    current_layout_shift = scancode_to_ascii_shift_qwerty;
}

void keyboard_set_layout(keyboard_layout_t layout) {
    if (layout == KEYBOARD_LAYOUT_QWERTZ) {
        current_layout_normal = scancode_to_ascii_qwertz;
        current_layout_shift = scancode_to_ascii_shift_qwertz;
    } else {
        current_layout_normal = scancode_to_ascii_qwerty;
        current_layout_shift = scancode_to_ascii_shift_qwerty;
    }
}

uint8_t keyboard_get_scancode() {
    while (!(inb(KEYBOARD_STATUS_PORT) & 0x1));
    return inb(KEYBOARD_DATA_PORT);
}

char keyboard_get_char() {
    uint8_t scancode = keyboard_get_scancode();
    
    // Handle key release (high bit set)
    if (scancode & 0x80) {
        scancode &= 0x7F;
        if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT) {
            shift_pressed = 0;
        }
        return 0;
    }
    
    // Handle key press
    if (scancode == KEY_LSHIFT || scancode == KEY_RSHIFT) {
        shift_pressed = 1;
        return 0;
    }
    
    if (scancode < 58) { // Size of our arrays
        if (shift_pressed) {
            return current_layout_shift[scancode];
        } else {
            return current_layout_normal[scancode];
        }
    }
    
    return 0;
}
