#include "lightwall.h"

#define DEBUG   true

#define LED_CHIP_SET    UCS1903
#define LED_CHIP_RGBSET BGR

void setup()
{
  Serial.begin(115200);

  PrintVersionInfo();
  
  Serial.println("Starting...");

  // ... 读取配置
  EEPROM.begin(1 + GetConfigNeedBytes() + GetMatrixNeedBytes());

  if (IsNewDevice())
  {
    Serial.print("This is a NEW device, resetting config to DEFAULT...");
    // 如果是新设备，则保存默认配置到EEPROM
    ResetConfigToDefault();
    SaveConfig();
    Serial.println("OK!");
  }
  else
  {
    Serial.print("Reading config from EEPROM...");
    // 否则读取早期保存的配置
    ReadConfig();
    Serial.println("OK!");
  }

  // ... LED 驱动初始化
  Serial.println("Initialize LED driver...");
  g_ledsNum = g_config.led_grid_config.led_grid_count;
  Serial.print("LEDs Number = "); Serial.println(g_ledsNum);
  gp_leds = new CRGB[g_ledsNum];
  FastLED.addLeds<LED_CHIP_SET, DATA_PIN, LED_CHIP_RGBSET>(gp_leds, g_ledsNum);

  LEDS.setBrightness(256);
  delay(500);
  LEDS.setBrightness(0);
  delay(500);
  LEDS.setBrightness(256);
  delay(500);
  LEDS.setBrightness(0);
  delay(500);
  LEDS.setBrightness(256);
  delay(500);
  LEDS.setBrightness(0);
  delay(500);

  Serial.println("OK!");

  // ... 初始化拾音器

  // ... 初始化Wifi

  // ... 初始化TCP Server
  
}

void loop()
{
  // . 检查串口消息
  if (Serial.available())
  {
    if (Serial.peek() == MSG_HEAD)    // Network message
    {
      ReceiveAndProcessSerialMessage();
    }
    else                              // AT Command
    {
      ReceiveAndProcessATCommand();
    }
  }

  // . 检查网络消息

  // . 运行当前模式
  switch (g_config.current_mode)
  {
    // 0: 默认： 呼吸灯
    case MODE_DEFAULT_Breathing:
      {
        if (g_needReinitializeMode)
        {
          Mode_0_Initialize();
          g_needReinitializeMode = false;
        }

        Mode_0_Process();
      }
      break;
    default:
      {
        // 配置错误，默认恢复到呼吸灯
        g_config.current_mode = MODE_DEFAULT_Breathing;
      }
      break;
  }

  LEDS.show();
  //delay(10);
}
