#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ArduinoJson.h>
#include <WiFi.h>

#define TFT_HOR_RES   480
#define TFT_VER_RES   320

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))

void *draw_buf;
unsigned long lastTickMillis = 0;

const char* ssid     = "iptime_2.4GHz";
const char* password = "1203002150";

// String Enapikey = "FEks%2F1vfHSzO0X0TrsZAKPG4tqk2FmTtLgvFZf3qYf91zAu4fzfZzXcFnxw7ejWwb73US%2FIsLaIgvrd3H9WoSQ%3D%3D";
// String Deapikey = "FEks/1vfHSzO0X0TrsZAKPG4tqk2FmTtLgvFZf3qYf91zAu4fzfZzXcFnxw7ejWwb73US/IsLaIgvrd3H9WoSQ==";

// String url = "http://apis.data.go.kr/1360000/EqkInfoService/getEqkMsg?ServiceKey=${Enapikey}&pageNo=1&numOfRows=10&dataType=JSON&fromTmFc=20240101&toTmFc=20240422";

void setup()
{
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.begin( 115200 );
    Serial.println( LVGL_Arduino );

    WiFi.begin(ssid, password);
    lv_init();

    while (WiFi.status() != WL_CONNECTED) {// 와이파이 접속하는 동안 "." 출력
      delay(5000);
      Serial.println(".");
    }
    draw_buf = heap_caps_malloc(DRAW_BUF_SIZE, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

    lv_display_t * disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, DRAW_BUF_SIZE);

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_text( label, LVGL_Arduino.c_str());
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

    Serial.println( "Setup done" );

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
  unsigned int tickPeriod = millis() - lastTickMillis;
  lv_tick_inc(tickPeriod);
  lastTickMillis = millis();
  lv_task_handler();
}
