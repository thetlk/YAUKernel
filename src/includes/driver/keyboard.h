#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_handle_code(unsigned char code);

#define KEYBOARD_PRESSED_KEY(code) (!(code & 0x80))

#endif