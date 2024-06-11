#ifndef DISPLAY_SERVICE_H
#define DISPLAY_SERVICE_H

#include "lvgl.h"
#include "TFT_eSPI.h"

class display_service
{
private:
  /* =============================icache functions============================= */
  void ICACHE_FLASH_ATTR lv_setup();
#ifdef TOUCHPAD
  void ICACHE_FLASH_ATTR touch_setup();
#endif
#ifdef KEYPAD
  void ICACHE_FLASH_ATTR keypad_setup();
#endif
  /* ============================end icache functions========================== */

  /* =============================iram functions=============================== */
  void IRAM_ATTR lv_main();
  static void IRAM_ATTR my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
#ifdef TOUCHPAD
  static void IRAM_ATTR my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
#endif
#ifdef KEYPAD
  static void IRAM_ATTR my_keypad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
#endif
  /* ============================end iram functions============================ */

public:
  display_service();
  ~display_service();

  /* =============================icache functions============================= */
  /* ============================end icache functions========================== */
  /* =============================iram functions=============================== */
  void IRAM_ATTR setup();
  void IRAM_ATTR loop();
  /* ============================end iram functions============================ */
};
#endif