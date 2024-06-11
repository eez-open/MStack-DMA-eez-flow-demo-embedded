#include "screens.h"

lv_group_t *keypad_group;

static lv_obj_tree_walk_res_t walk_cb(lv_obj_t *obj, void *) {
    if (obj->class_p == &lv_btn_class || obj->class_p == &lv_keyboard_class) {
        lv_group_add_obj(keypad_group, obj);
        if (lv_group_get_focused(keypad_group) == 0) {
            lv_group_focus_obj(obj);
        }
    }
    return LV_OBJ_TREE_WALK_NEXT;
}

static void update_keypad_group(lv_obj_t *obj) {
    lv_group_remove_all_objs(keypad_group);
    lv_obj_tree_walk(obj, walk_cb, 0);
}

static void on_screen_loaded(lv_event_t *e) {
    update_keypad_group(e->target);
}

static void on_keyboard_apply(lv_event_t *e) {
    lv_group_focus_next(keypad_group);
}

void ui_init_input_groups() {
    keypad_group = lv_group_create();
    lv_group_set_default(keypad_group);

    lv_obj_add_event_cb(objects.main, on_screen_loaded, LV_EVENT_SCREEN_LOADED, 0);
    lv_obj_add_event_cb(objects.page1, on_screen_loaded, LV_EVENT_SCREEN_LOADED, 0);
    lv_obj_add_event_cb(objects.page2, on_screen_loaded, LV_EVENT_SCREEN_LOADED, 0);

    lv_obj_add_event_cb(objects.keyboard, on_keyboard_apply, LV_EVENT_READY, 0);

    update_keypad_group(objects.main);
}