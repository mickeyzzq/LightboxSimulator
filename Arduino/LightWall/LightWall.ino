#include "lightwall.h"

#define DEBUG   true

#define LED_CHIP_SET    WS2811 //UCS1903
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
  g_ledsNum = g_config.led_grid_config.led_grid_count * g_config.led_grid_config.ic_num_per_grid;
  Serial.print("LEDs Number = "); Serial.println(g_ledsNum);
  //gp_leds = new CRGB[g_ledsNum];
  FastLED.addLeds<LED_CHIP_SET, DATA_PIN, LED_CHIP_RGBSET>(gp_leds, g_ledsNum);

  for (int i = 0; i < g_ledsNum; i++)
  {
    gp_leds[i] = CRGB::White;
  }
  LEDS.setBrightness(256);
  LEDS.show();
  delay(500);
  LEDS.setBrightness(0);
  LEDS.show();
  delay(500);
  LEDS.setBrightness(256);
  LEDS.show();
  delay(500);
  LEDS.setBrightness(0);
  LEDS.show();

  g_needReinitializeMode = true;

  Serial.println("OK!");

  // ... 初始化拾音器
  MicrophoneSetup();
  
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
    // 4: 默认： MODE_VOCAL_Brightness
    case MODE_VOCAL_Brightness:
      {
        if (g_needReinitializeMode)
        {
          Mode_4_Initialize();
          g_needReinitializeMode = false;
        }

        Mode_4_Process();
      }
      break;
    default:
      {
        // 配置错误，默认恢复到呼吸灯
        g_config.current_mode = MODE_DEFAULT_Breathing;
      }
      break;
  }

  delay(20);
  LEDS.show();
}
