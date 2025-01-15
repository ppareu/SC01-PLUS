#include <lvgl.h>
#include <TFT_eSPI.h>

#define TFT_HOR_RES   480
#define TFT_VER_RES   320

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

void *draw_buf;
unsigned long lastTickMillis = 0;

// void my_touch_read(lv_indev_t *indev, lv_indev_data_t *data) {
//   if(tft.touched()){
//     TS_Point p = tft.getPoint();
//     data->point.x = TFT_HOR_RES - p.y;
//     data->point.y = p.x;
//     Serial.printf("x: %d, y: %d\n", data->point.x, data->point.y);
//     data->state = LV_INDEV_STATE_PRESSED;
//   } else {
//     data->state = LV_INDEV_STATE_RELEASED;
//   }
// }

void setup()
{
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.begin( 115200 );
    Serial.println( LVGL_Arduino );

    // if(!tft.begin(40)){
    //   Serial.println("Unable to start the capacitive touch screen");
    // }

    lv_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    // lv_tick_set_cb(millis);

    draw_buf = heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

    lv_display_t * disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);

    // lv_indev_t *indev = lv_indev_create();
    // lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    // lv_indev_set_read_cb(indev, my_touch_read);

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text( label, LVGL_Arduino.c_str());
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    Serial.println( "Setup done" );
}

void loop()
{
  unsigned int tickPeriod = millis() - lastTickMillis;
  lv_tick_inc(tickPeriod);
  lastTickMillis = millis();
  lv_task_handler();
}
