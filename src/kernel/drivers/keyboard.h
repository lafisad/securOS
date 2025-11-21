#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../utils/types.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define KEY_BACKSPACE 0x0E
#define KEY_ENTER 0x1C
#define KEY_LSHIFT 0x2A
#define KEY_RSHIFT 0x36
#define KEY_LCTRL 0x1D
#define KEY_LALT 0x38

typedef enum {
    KEYBOARD_LAYOUT_QWERTY,
    KEYBOARD_LAYOUT_QWERTZ
} keyboard_layout_t;

void keyboard_init();
void keyboard_set_layout(keyboard_layout_t layout);
char keyboard_get_char();
uint8_t keyboard_get_scancode();

#endif
