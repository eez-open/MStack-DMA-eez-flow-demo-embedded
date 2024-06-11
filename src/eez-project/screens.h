#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *page1;
    lv_obj_t *page2;
    lv_obj_t *eez_logo;
    lv_obj_t *formatted_textarea;
    lv_obj_t *home_button_1;
    lv_obj_t *home_button_2;
    lv_obj_t *keyboard;
    lv_obj_t *page1_button;
    lv_obj_t *page2_button;
    lv_obj_t *txtarea1;
    lv_obj_t *value_textarea;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_PAGE1 = 2,
    SCREEN_ID_PAGE2 = 3,
};

void create_screen_main();
void tick_screen_main();

void create_screen_page1();
void tick_screen_page1();

void create_screen_page2();
void tick_screen_page2();

void create_screens();
void tick_screen(int screen_index);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/