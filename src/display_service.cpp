#include <Arduino.h>
#include "config.h"
#include "display_service.h"
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "esp_freertos_hooks.h"
#include "demos/lv_demos.h"
#include "demos/widgets/lv_demo_widgets.h"
#include "demos/benchmark/lv_demo_benchmark.h"
#include "demos/stress/lv_demo_stress.h"
#include "demos/music/lv_demo_music.h"
#include "demos/keypad_encoder/lv_demo_keypad_encoder.h"

#include "eez-project/ui.h"

#ifdef KEYPAD
#include "keypad.h"
#endif

extern TFT_eSPI tft = TFT_eSPI(); //load tft service

display_service::display_service() {}
display_service::~display_service() {}

static void lv_tick_task();
static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf1[DISP_BUF_SIZE];

#if (BUF_NUM== 2)
static lv_color_t buf2[DISP_BUF_SIZE];
#endif

/* =============================icache functions========================= */
#ifdef TOUCHPAD
void ICACHE_FLASH_ATTR display_service::touch_setup()
{

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register( &indev_drv ); 
}
#endif

#ifdef KEYPAD
extern lv_group_t *keypad_group;
void ICACHE_FLASH_ATTR display_service::keypad_setup()
{
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_KEYPAD;
  indev_drv.read_cb = my_keypad_read;
  lv_indev_t *keypad_indev = lv_indev_drv_register( &indev_drv );

  lv_indev_set_group(keypad_indev, keypad_group);
}
#endif

static void lv_tick_task()
{   
   lv_tick_inc(portTICK_PERIOD_MS);
}
void ICACHE_FLASH_ATTR display_service::lv_setup()
{
  lv_init();

  tft.begin();               /* TFT init */
  tft.invertDisplay(1);
  tft.setRotation(ROTATION); /* Landscape orientation */
  tft.initDMA();
#if (BUF_NUM == 1)
  // lv_disp_buf_init(&disp_buf, buf1, NULL, DISP_BUF_SIZE);
  lv_disp_draw_buf_init( &disp_buf, buf1, NULL, DISP_BUF_SIZE );
#elif (BUF_NUM == 2)
  lv_disp_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);
#endif

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = LV_HOR_RES_MAX;
  disp_drv.ver_res = LV_VER_RES_MAX;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.full_refresh = 0;
  disp_drv.draw_buf = &disp_buf;
  lv_disp_drv_register(&disp_drv);
   
  //lv_demo_widgets();
  ui_init();

  extern void ui_init_input_groups();
  ui_init_input_groups();

// #ifdef _DEBUG_
//   Serial.print(F("[INFO] Display GUI setup finished! \n"));
// #endif
}

void ICACHE_FLASH_ATTR display_service::setup()
{
  lv_setup();

  esp_register_freertos_tick_hook(lv_tick_task);

#ifdef _DEBUG_
  Serial.print(F("[INFO] Display GUI setup finished! \n"));
#endif
#ifdef TOUCHPAD
  touch_setup();
#endif
#ifdef KEYPAD
  keypad_setup();
#endif

  // lv_main();

} // end display service setup

/* =========================end icache functions======================== */

/* =========================== iram functions=========================== */
#ifdef TOUCHPAD
void IRAM_ATTR display_service::my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{

     bool touched = tp.get_touch(50);
    if (tp.get_xy.touch == 0)
    {
        return ;
    }

    if (tp.get_xy.x > LV_HOR_RES_MAX || tp.get_xy.y > LV_VER_RES_MAX)
    {
#ifdef _DEBUG_
        Serial.printf("[INFO][DISPLAY][TOUCH][ERR]Y or y outside of expected parameters.. X=%d, Y=%d\n", tp.get_xy.x, tp.get_xy.y);
#endif
    }
    else
    {

        data->state = (tp.get_xy.touch == true) ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;
        // data->state = LV_INDEV_STATE_PR;

        /*Save the state and save the pressed coordinate*/
        //if(data->state == LV_INDEV_STATE_PR) touchpad_get_xy(&last_x, &last_y);
        if (data->state == LV_INDEV_STATE_PR)
        {
            data->point.x = tp.get_xy.x;
            data->point.y = tp.get_xy.y;
#ifdef _DEBUG_
            Serial.printf("[INFO][TP] X is %d \n", tp.get_xy.x);
            Serial.printf("[INFO][TP] Y is %d \n", tp.get_xy.y);
            Serial.printf("[INFO][TP] Touch status is %d \n", tp.get_xy.touch);
            Serial.printf("[INFO][UPTIME] %ld \n", millis());
            Serial.printf("[INFO][MEM] FREE memory %d \n", ESP.getFreeHeap());
#endif
        }
    }

    return ; /*Return `false` because we are not buffering and no more data to read*/
}
#endif

#ifdef KEYPAD
void IRAM_ATTR display_service::my_keypad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    uint32_t button;
    bool isPressed;
    m5stack_keypad_read(button, isPressed);

    bool isKeyboardWidgetFocused = false;
    lv_obj_t *focusedObj = lv_group_get_focused(keypad_group);
    if (focusedObj && focusedObj->class_p == &lv_keyboard_class) {
        isKeyboardWidgetFocused = true;
    }

    data->key = 
        button == MY_BUTTON_A ? (isKeyboardWidgetFocused ? LV_KEY_LEFT : LV_KEY_PREV) :
        button == MY_BUTTON_B ? (isKeyboardWidgetFocused ? LV_KEY_RIGHT : LV_KEY_NEXT) :
        button == MY_BUTTON_C ? LV_KEY_ENTER :
        0;

    if (isPressed) {
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}
#endif

void IRAM_ATTR display_service::lv_main()
{

#ifdef _DEBUG_
  Serial.print(F("[INFO] LV GUI started.\n"));
#endif
    // header_create();
    // body_create();

}

void IRAM_ATTR display_service::loop()
{

  lv_timer_handler(); /* let the GUI do its work */
  ui_tick();
    delay( 5 );


  // lv_task_handler(); /* let the GUI do its work */
    // delay( 5 );
} //end loop

/* Display flushing */
void IRAM_ATTR display_service::my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.startWrite();
    //tft.pushColors(&color_p->full, w * h, true);
    tft.setSwapBytes(true);
    tft.pushPixelsDMA(&color_p->full, w * h); // Push line to screen
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/* ===========================end iram functions=========================== */