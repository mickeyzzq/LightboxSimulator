#include <Arduino.h>
#include "lightwall.h"

LedGridConfig& ledCfg = g_config.led_grid_config;

void LedGrid_SetColor(uint16_t gridIndex, const CRGB& color)
{
  if (gridIndex >= ledCfg.led_grid_count)
  {
    return;
  }

  int startIc = gridIndex * ledCfg.ic_num_per_grid;
  int endIc = startIc + ledCfg.ic_num_per_grid;
  
  for (int icIdx = startIc; icIdx < endIc; icIdx++)
  {
    gp_leds[icIdx] = color;
  }
}

void LedGrid_ClearColor()
{
  for (int i = 0; i < g_ledsNum; i++)
  {
    gp_leds[i] = 0x000000;
  }
}
