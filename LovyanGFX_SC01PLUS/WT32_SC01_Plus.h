#ifndef WT32_SC01_PLUS_H
#define WT32_SC01_PLUS_H

#include <LovyanGFX.hpp>
#define TFT_WIDTH   320
#define TFT_HEIGHT  480

class LGFX : public lgfx::LGFX_Device
{

  lgfx::Panel_ST7796   _panel_instance; // ST7796UI
  lgfx::Bus_Parallel8  _bus_instance;   // MCU 8080 8비트
  lgfx::Light_PWM      _light_instance;
  lgfx::Touch_FT5x06   _touch_instance;

public:
  LGFX(void)
  {
    { // 버스 제어 설정을 합니다.
      auto cfg = _bus_instance.config();    // 버스 설정용 구조체를 가져옵니다.

      // 8비트 병렬 버스 설정
      cfg.freq_write = 40000000;    // 전송 클럭 (최대 20MHz, 80MHz를 정수로 나눈 값으로 반올림 됩니다)
      cfg.pin_wr = 47;              // WR에 연결된 핀 번호
      cfg.pin_rd = -1;              // RD에 연결된 핀 번호
      cfg.pin_rs =  0;              // RS(D/C)에 연결된 핀 번호

      // LCD data interface, 8bit MCU (8080)
      cfg.pin_d0 =  9;   // D0에 연결된 핀 번호
      cfg.pin_d1 = 46;  // D1에 연결된 핀 번호
      cfg.pin_d2 =  3;   // D2에 연결된 핀 번호
      cfg.pin_d3 =  8;   // D3에 연결된 핀 번호
      cfg.pin_d4 = 18;  // D4에 연결된 핀 번호
      cfg.pin_d5 = 17;  // D5에 연결된 핀 번호
      cfg.pin_d6 = 16;  // D6에 연결된 핀 번호
      cfg.pin_d7 = 15;  // D7에 연결된 핀 번호


      _bus_instance.config(cfg);               // 설정 값을 버스에 반영합니다.
      _panel_instance.setBus(&_bus_instance);  // 버스를 패널에 설정합니다.
    }

    { // 디스플레이 패널 제어 설정을 합니다.
      auto cfg = _panel_instance.config();    // 디스플레이 패널 설정용 구조체를 가져옵니다.

      cfg.pin_cs           =    -1;  // CS가 연결된 핀 번호   (-1 = 비활성화)
      cfg.pin_rst          =     4;  // RST가 연결된 핀 번호  (-1 = 비활성화)
      cfg.pin_busy         =    -1;  // BUSY가 연결된 핀 번호 (-1 = 비활성화)

      cfg.panel_width      =   TFT_WIDTH;  // 실제로 표시 가능한 너비
      cfg.panel_height     =   TFT_HEIGHT;  // 실제로 표시 가능한 높이
      cfg.offset_x         =     0;  // 패널의 X 방향 오프셋 양
      cfg.offset_y         =     0;  // 패널의 Y 방향 오프셋 양
      cfg.offset_rotation  =     0;  // 회전 방향의 값 오프셋 0~7 (4~7은 상하 반전)
      cfg.dummy_read_pixel =     8;  // 픽셀 읽기 전의 더미 리드 비트 수
      cfg.dummy_read_bits  =     1;  // 픽셀 외 데이터 읽기 전의 더미 리드 비트 수
      cfg.readable         =  true;  // 데이터 읽기가 가능한 경우 true로 설정
      cfg.invert           =  true;  // 패널의 밝기가 반전되는 경우 true로 설정
      cfg.rgb_order        = false;  // 패널의 빨강과 파랑이 바뀌는 경우 true로 설정
      cfg.dlen_16bit       = false;  // 16비트 병렬 또는 SPI로 데이터 길이를 16비트 단위로 전송하는 패널의 경우 true로 설정
      cfg.bus_shared       =  true;  // SD카드와 버스를 공유하는 경우 true로 설정(drawJpgFile 등에서 버스 제어를 합니다)

// ST7735나 ILI9163 같이 픽셀 수가 가변적인 드라이버에서 표시가 어긋나는 경우에만 설정하세요.
//    cfg.memory_width     =   TFT_WIDTH;  // 드라이버 IC가 지원하는 최대 너비
//    cfg.memory_height    =   TFT_HEIGHT;  // 드라이버 IC가 지원하는 최대 높이

      _panel_instance.config(cfg);
    }

    { // 백라이트 제어 설정을 합니다. (필요 없으면 삭제)
      auto cfg = _light_instance.config();    // 백라이트 설정용 구조체를 가져옵니다.

      cfg.pin_bl = 45;              // 백라이트가 연결된 핀 번호
      cfg.invert = false;           // 백라이트의 밝기를 반전시키는 경우 true
      cfg.freq   = 44100;           // 백라이트의 PWM 주파수
      cfg.pwm_channel = 7;          // 사용할 PWM의 채널 번호

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  // 백라이트를 패널에 설정합니다.
    }


    { // 터치스크린 제어 설정을 합니다. (필요 없으면 삭제)
      auto cfg = _touch_instance.config();

      cfg.x_min      = 0;    // 터치스크린에서 얻을 수 있는 최소 X값(원시 값)
      cfg.x_max      = 319;  // 터치스크린에서 얻을 수 있는 최대 X값(원시 값)
      cfg.y_min      = 0;    // 터치스크린에서 얻을 수 있는 최소 Y값(원시 값)
      cfg.y_max      = 479;  // 터치스크린에서 얻을 수 있는 최대 Y값(원시 값)
      cfg.pin_int    = 7;   // INT가 연결된 핀 번호
      cfg.bus_shared = true; // 화면과 공통 버스를 사용하는 경우 true로 설정
      cfg.offset_rotation = 0;// 표시와 터치의 방향이 일치하지 않는 경우의 조정 0~7값으로 설정

      // I2C 연결
      cfg.i2c_port = 1;      // 사용할 I2C 선택 (0 또는 1)
      cfg.i2c_addr = 0x38;   // I2C 장치 주소 번호
      cfg.pin_sda  = 6;     // SDA가 연결된 핀 번호
      cfg.pin_scl  = 5;     // SCL이 연결된 핀 번호
      cfg.freq = 400000;     // I2C 클럭 설정

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);  // 터치스크린을 패널에 설정합니다.
    }

    setPanel(&_panel_instance); // 사용할 패널을 설정합니다.
  }
};

#endif
