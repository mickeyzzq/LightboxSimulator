#include <Arduino.h>
#include "lightwall.h"

//==================================================
// 模式： 0： 默认： 基础-呼吸灯
//   参数意义说明：
//      速度0：0~60，值=0时：停止； 值>0时，为每个呼吸循环(暗->亮->暗)的秒数，最大值为60
//      幅度0：最低亮度，0~1，硬件值为 255 * a
//      幅度1：最高亮度，0~1，硬件值为 255 * a
//   子模式raw[0]: 0 - 亮度呼吸，变亮度
//                1 - 亮度呼吸，全屏变色，变暗时插值到下一个颜色，随机色彩
//                2 - 亮度呼吸，全屏变色，变暗时插值到下一个颜色，指定色彩循环
//                3 - 变色呼吸，直接插值变化到下一个颜色，随机或指定色彩循环
#define MAX_MODE_0_SUBMODE    4
// 参数
#define M0_MAX_SPEED                          60
uint8_t m_brightness_min = 1;
uint8_t m_brightness_max = 255;
uint8_t m_subMode = 0;

void Mode_0_Sub_0_Process();
void Mode_0_Sub_1_Process();
void Mode_0_Sub_2_Process();
void Mode_0_Sub_3_Process();

// 运行时间
unsigned long m_millis_per_cycle            = 0; // 每循环用millis
unsigned long m_millis_current_cycle_start  = 0; // 本循环的起始millis

void Mode_0_Initialize()
{
  Serial.println("Initialize Mode 0...");
  ModeParams& myParams = g_config.mode_params[MODE_DEFAULT_Breathing];
  // . 子模式
  m_subMode = myParams.raw[0];
  if (m_subMode < 0 || m_subMode >= MAX_MODE_0_SUBMODE)
  {
    m_subMode = 0;
  }

  // . 计算速度参数
  m_millis_per_cycle = 1000 * myParams.speed[0];
  if (myParams.speed[0] <= 0) // 为零时，显示为最亮状态，不变化
    m_millis_per_cycle = 0;
  else if (myParams.speed[0] > M0_MAX_SPEED)
    m_millis_per_cycle = 1000 * M0_MAX_SPEED;
  Serial.print("m_millis_per_cycle = "); Serial.println(m_millis_per_cycle);

  // . 亮度参数
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
  switch (m_subMode)
  {
    case 0:     // 单恋亮度的呼吸灯
    {
      Mode_0_Sub_0_Process();
    }
    break;
    case 1:
    {
      Mode_0_Sub_0_Process();
    }
    break;
    case 2:
    {
      Mode_0_Sub_0_Process();
    }
    break;
    case 3:
    {
      Mode_0_Sub_0_Process();
    }
    break;
    default:
    {
      
    }
    break;
  }
}

// -------------------------------------------
// 0 - 亮度呼吸，变亮度，颜色由color[0]唯一指定
void Mode_0_Sub_0_Process()
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
    Serial.print("SubMode = ");Serial.println(m_subMode);
    Serial.print("\tlerp_point="); Serial.print(lerp_point);
    Serial.print("\tmillis_per_half_cycle="); Serial.print(millis_per_half_cycle);
    Serial.print("\tm_brightness_min="); Serial.print(m_brightness_min);
    Serial.print("\tm_brightness_max="); Serial.print(m_brightness_max);
    Serial.print("\tbright="); Serial.print(bright);
    Serial.println();
    if (millis() > g_print_mode_stoptime)
    {
      g_print_mode_info = false;
      Serial.println("Down.");
    }
  }
  //delay(20);
}

void Mode_0_Sub_1_Process()
{
  
}

void Mode_0_Sub_2_Process()
{
  
}

void Mode_0_Sub_3_Process()
{
  
}
