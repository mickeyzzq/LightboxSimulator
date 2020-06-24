#include <Arduino.h>
#include "lightwall.h"

//==================================================
// 模式： 0： 默认： 基础-呼吸灯
//   参数意义说明：
//      速度0：0~60，值=0时：停止； 值>0时，为每个呼吸循环(暗->亮->暗)的秒数，最大值为60
//      幅度0：最低亮度，0~1，硬件值为 255 * a
//      幅度1：最高亮度，0~1，硬件值为 255 * a

// 参数
#define M0_MAX_SPEED                          60
uint8_t m_brightness_min = 1;
uint8_t m_brightness_max = 255;
int m_mIdx = MODE_DEFAULT_Breathing;

// 运行时间
unsigned long m_millis_per_cycle            = 0; // 每循环用millis
unsigned long m_millis_current_cycle_start  = 0; // 本循环的起始millis

void Mode_0_Initialize()
{
  Serial.println("Initialize Mode 0...");
  ModeParams& myParams = g_config.mode_params[m_mIdx];
  // 0. 计算参数
  m_millis_per_cycle = 1000 * myParams.speed[0];
  if (myParams.speed[0] <= 0) // 为零时，显示为最亮状态，不变化
    m_millis_per_cycle = 0;
  else if (myParams.speed[0] > M0_MAX_SPEED)
    m_millis_per_cycle = 1000 * M0_MAX_SPEED;
  Serial.print("m_millis_per_cycle = "); Serial.println(m_millis_per_cycle);

  if (myParams.amplitude[0] <= 0 || myParams.amplitude[0] > 1)
    m_brightness_min = 1;
  else
    m_brightness_min = 255 * myParams.amplitude[0];
  Serial.print("m_brightness_min = "); Serial.println(m_brightness_min);

  if (myParams.amplitude[1] <= 0 || myParams.amplitude[1] > 1)
    m_brightness_max = 254;
  else
    m_brightness_max = 254 * myParams.amplitude[1];
  Serial.print("m_brightness_max = "); Serial.println(m_brightness_max);

  // 设置颜色
  for (int i = 0; i < g_ledsNum; i++)
  {
    gp_leds[i] = myParams.color[0];
  }
  Serial.print("led color set to: 0x"); Serial.println(myParams.color[0], HEX);

  // 本模式的启动时间
  g_mode_start_millis = millis();

  // 本呼吸循环的启动时间（如果是在循环期间修改的参数，则不重置本循环启动时间）
  if (g_mode_start_millis - m_millis_current_cycle_start > m_millis_per_cycle)
  {
    m_millis_current_cycle_start = g_mode_start_millis;
  }
  Serial.println("Initialize OK.");
}

void Mode_0_Process()
{
  // 按速度和幅度配置循环
  unsigned long current_millis = millis();
  unsigned long delta = current_millis - m_millis_current_cycle_start;
  if (delta > m_millis_per_cycle)
  {
    m_millis_current_cycle_start = current_millis;
    delta = 0;
  }

  unsigned long millis_per_half_cycle = m_millis_per_cycle / 2;
  unsigned long lerp_point = delta;
  // 前半段从暗变亮
  if (delta > millis_per_half_cycle)
  {
    lerp_point = m_millis_per_cycle - delta;
  }

  int bright = map(lerp_point, 0, millis_per_half_cycle, m_brightness_min, m_brightness_max);
  LEDS.setBrightness(bright);

  if (g_print_mode_info)
  {
    Serial.print("\tlerp_point=");Serial.print(lerp_point);
    Serial.print("\tmillis_per_half_cycle=");Serial.print(millis_per_half_cycle);
    Serial.print("\tm_brightness_min=");Serial.print(m_brightness_min);
    Serial.print("\tm_brightness_max=");Serial.print(m_brightness_max);
    Serial.print("\tbright=");Serial.print(bright);
    Serial.println();
    if (millis() > g_print_mode_stoptime)
    {
      g_print_mode_info = false;
      Serial.println("Down.");
    }
  }
  delay(20);
}
