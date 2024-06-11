#pragma once

#ifdef KEYPAD

#include <stdbool.h>

#define MY_BUTTON_NONE 0
#define MY_BUTTON_A 1
#define MY_BUTTON_B 2
#define MY_BUTTON_C 3

void m5stack_keypad_read(uint32_t &button, bool &isPressed);

#endif
