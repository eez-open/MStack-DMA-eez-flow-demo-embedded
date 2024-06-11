#include <Arduino.h>
#include <M5Stack.h>

#include "config.h"
#include "keypad.h"

static uint32_t lastButton = MY_BUTTON_NONE;

#ifdef KEYPAD
void m5stack_keypad_read(uint32_t &button, bool &isPressed) {
    M5.update();

    if (lastButton != MY_BUTTON_NONE) {
        if (lastButton == MY_BUTTON_A) {
            isPressed = M5.BtnA.isPressed();
        } else if (lastButton == MY_BUTTON_B) {
            isPressed = M5.BtnB.isPressed();
        } else if (lastButton == MY_BUTTON_C) {
            isPressed = M5.BtnC.isPressed();
        }
    } else {
        isPressed = true;
        if (M5.BtnA.isPressed()) {
            lastButton = MY_BUTTON_A;
        } else if (M5.BtnB.isPressed()) {
            lastButton = MY_BUTTON_B;
        } else if (M5.BtnC.isPressed()) {
            lastButton = MY_BUTTON_C;
        } else {
            isPressed = false;
        }
    }

    button = lastButton;

    if (!isPressed) {
        lastButton = MY_BUTTON_NONE;
    }
}
#endif
