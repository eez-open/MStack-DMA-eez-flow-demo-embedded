#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_PREFIXES = 0
};

// Native global variables



void init_vars();
void tick_vars();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/