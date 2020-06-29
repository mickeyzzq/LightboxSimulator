#include <Arduino.h>
#include "lightwall.h"

uint16_t rand_grid_indexs[10][10];
CRGB colors[] = {CRGB::White, CRGB::AliceBlue, CRGB::Amethyst, CRGB::DeepPink, CRGB::LightYellow, CRGB::Green, CRGB::LightSalmon, CRGB::DodgerBlue, CRGB::Chartreuse, CRGB::DarkKhaki };

void Mode_4_Initialize()
{
  for (int x = 0; x < 10; x++)
  {
    for(int y = 0; y < 10; y++)
    {
      rand_grid_indexs[x][y] = random(g_config.led_grid_config.led_grid_count);
    }
  }
}

void Mode_4_Process()
{
  // 第3秒开始
  delay(5000);

  // 三声喊，每声持续1秒，间隔1秒
  for (int st = 0; st < 3; st++)
  {
    LedGrid_ClearColor();
    delay(20);
    LEDS.show();
      
    for (int i = 0; i < 10; i++)
    {
      LedGrid_SetColor(rand_grid_indexs[st][i], colors[st]);
    }

    uint8_t bright = 0;
    uint8_t delta = 3;
    while (bright < 200)
    {
      LEDS.setBrightness(bright);
      delay(20);
      LEDS.show();
      if (bright > 32) delta += 2;
      if (bright > 64) delta += 2;
      bright += delta;
      delay(20);
    }
    LEDS.setBrightness(0);
    delay(20);
    LEDS.show();
    delay(1000);
  }

  

  // 等3秒
  LEDS.setBrightness(0);
  delay(20);
  LEDS.show();
  delay(2000);

  
  // 三声喊，每声持续1秒，间隔0秒
  for (int st = 3; st < 6; st++)
  {
    LedGrid_ClearColor();
    delay(20);
    LEDS.show();
      
    for (int i = 0; i < 10; i++)
    {
      LedGrid_SetColor(rand_grid_indexs[st][i], colors[st]);
    }

    uint8_t bright = 0;
    uint8_t delta = 3;
    while (bright < 200)
    {
      LEDS.setBrightness(bright);
      delay(20);
      LEDS.show();
      if (bright > 32) delta += 2;
      if (bright > 64) delta += 2;
      bright += delta;
      delay(20);
    }
  }
  LEDS.setBrightness(0);
  delay(20);
  LEDS.show();

}
