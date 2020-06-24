#include <Arduino.h>
#include "lightwall.h"

// . 全局变量：
Config          g_config;                       // 配置表
unsigned int    g_ledsNum               = 1;
CRGB*           gp_leds                 = NULL; // LED对象数组
bool            g_needReinitializeMode  = false;
unsigned long   g_mode_start_millis     = 0;
bool            g_print_mode_info       = false;
unsigned int    g_print_mode_stoptime   = 0;

// 本地函数
void PrintConfigParam_raw(int modeIdx);
void PrintConfigParam_color(int modeIdx);
void PrintConfigParam_speed(int modeIdx);
void PrintConfigParam_amplitude(int modeIdx);

size_t GetConfigNeedBytes()
{
  return sizeof(g_config);
}

size_t GetMatrixNeedBytes()
{
  if (g_config.led_grid_config.row_num > 0 && g_config.led_grid_config.col_num > 0)
    return g_config.led_grid_config.row_num * g_config.led_grid_config.col_num;
  else
    return DEFAUTL_ROW_NUM * DEFAULT_COL_NUM;
}

void PrintStoregeInfo()
{
  // 从EEPROM读取配置
  size_t size_of_config = GetConfigNeedBytes();
  size_t size_of_matrix = GetMatrixNeedBytes();
  Serial.print("Size of EEPROM storage space required:"); Serial.println(1 + size_of_config + size_of_matrix);
  Serial.println("\tSize of save flag: 1");
  Serial.print("\tSize of config: "); Serial.println(size_of_config);
  Serial.print("\tSize of matrix: "); Serial.println(size_of_matrix);
}

bool IsNewDevice()
{
  return (EEPROM.read(0) != SAVED_FLAG);
}

void ResetConfigToDefault()
{
  memset(&g_config, 0, sizeof(g_config));

  g_config.led_grid_config.led_grid_count   = 60;
  g_config.led_grid_config.ic_num_per_grid  = 8;
  g_config.led_grid_config.row_num          = DEFAUTL_ROW_NUM;
  g_config.led_grid_config.col_num          = DEFAULT_COL_NUM;
  // 模式0 默认参数
  g_config.mode_params[0].raw[0]            = 0;            // 呼吸模式：简单模式
  g_config.mode_params[0].color[0]          = 0xFFFFFF;     // 颜色：白色
  g_config.mode_params[0].speed[0]          = 12;           // 速度：中速
  g_config.mode_params[0].amplitude[0]      = 0.0;          // 幅度底：最小
  g_config.mode_params[0].amplitude[1]      = 0.6;          // 幅度峰：最大
  // 模式1 默认参数
  g_config.mode_params[1].raw[0]            = 0;            // 马赛克：简单模式
  g_config.mode_params[1].raw[1]            = 64;           // 马赛克：最小亮度
  g_config.mode_params[1].raw[2]            = 198;          // 马赛克：最大亮度
  g_config.mode_params[1].color[0]          = 0xFFFFFF;     // 颜色1：白色
  g_config.mode_params[1].color[1]          = 0xFF00FF;     // 颜色2：紫色
  g_config.mode_params[1].speed[0]          = 15;           // 速度：中速
  g_config.mode_params[1].amplitude[0]      = 0.1;          // 幅度底：最小
  g_config.mode_params[1].amplitude[1]      = 0.9;          // 幅度峰：最大
}

void SaveConfig()
{
  EEPROM.write(0, SAVED_FLAG);
  EEPROM.put(sizeof(SAVED_FLAG), g_config);

  //TODO: 写入阵列数据

  EEPROM.commit();
}

void ReadConfig()
{
  EEPROM.get(sizeof(SAVED_FLAG), g_config);

  // TODO: 读取阵列数据

}

//  Mode  Param   Idx   Value
//    0     raw     0     0
//    0     raw     1     4
//    0     color   0     0xFFFFFF
//    0     color   1     0xCFCFCF
//    0     color   3     0x122334

void PrintConfig()
{
  Serial.println("----------------------------------------------------------------");
  Serial.println("| Current config values:");
  Serial.print("| current_mode                    ="); Serial.println(g_config.current_mode);
  Serial.print("| led_grid_config.led_grid_count  = "); Serial.println(g_config.led_grid_config.led_grid_count);
  Serial.print("| led_grid_config.ic_num_per_grid = "); Serial.println(g_config.led_grid_config.ic_num_per_grid);
  Serial.print("| led_grid_config.row_num         = "); Serial.println(g_config.led_grid_config.row_num);
  Serial.print("| led_grid_config.col_num         = "); Serial.println(g_config.led_grid_config.col_num);
  Serial.print("| Test CRGB::White = 0x"); Serial.println(CRGB::White, HEX);
  Serial.println("| Mode\tParam\t\tIdx\tValue");
  for (int modeIdx = 0; modeIdx < MODE_COUNT; modeIdx++)
  {
    PrintConfigParam_raw(modeIdx);
    PrintConfigParam_color(modeIdx);
    PrintConfigParam_speed(modeIdx);
    PrintConfigParam_amplitude(modeIdx);
  }
  Serial.println("----------------------------------------------------------------");
}

void PrintConfigParam_raw(int modeIdx)
{
  for (int paramIdx = 0; paramIdx < PARAM_NUM; paramIdx++)
  {
    Serial.print("| "); Serial.print(modeIdx);
    Serial.print("\traw");
    Serial.print("\t\t"); Serial.print(paramIdx);
    Serial.print("\t\t"); Serial.print(g_config.mode_params[modeIdx].raw[paramIdx]);
    Serial.println();
  }
}

void PrintConfigParam_color(int modeIdx)
{
  for (int paramIdx = 0; paramIdx < PARAM_NUM; paramIdx++)
  {
    Serial.print("| "); Serial.print(modeIdx);
    Serial.print("\tcolor");
    Serial.print("\t\t"); Serial.print(paramIdx);
    Serial.print("\t\t0x");
    Serial.print(g_config.mode_params[modeIdx].color[paramIdx].r, HEX);
    Serial.print(g_config.mode_params[modeIdx].color[paramIdx].g, HEX);
    Serial.print(g_config.mode_params[modeIdx].color[paramIdx].b, HEX);
    Serial.println();
  }
}

void PrintConfigParam_speed(int modeIdx)
{
  for (int paramIdx = 0; paramIdx < PARAM_NUM; paramIdx++)
  {
    Serial.print("| "); Serial.print(modeIdx);
    Serial.print("\tspeed");
    Serial.print("\t\t"); Serial.print(paramIdx);
    Serial.print("\t\t"); Serial.print(g_config.mode_params[modeIdx].speed[paramIdx], 3);
    Serial.println();
  }
}

void PrintConfigParam_amplitude(int modeIdx)
{
  for (int paramIdx = 0; paramIdx < PARAM_NUM; paramIdx++)
  {
    Serial.print("| "); Serial.print(modeIdx);
    Serial.print("\tamplitude ");
    Serial.print("\t"); Serial.print(paramIdx);
    Serial.print("\t\t"); Serial.print(g_config.mode_params[modeIdx].amplitude[paramIdx], 3);
    Serial.println();
  }
}
