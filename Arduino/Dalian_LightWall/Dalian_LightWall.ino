#include <EEPROM.h>
#include <ArduinoJson.h>
#include <FastLED.h>
//#include "lightwall.h"
//
// 大连LED灯墙项目
// 0.概述：
//    0.1 此灯墙为12行23列的格子组成，但仅其中的61个格子中有灯光
//    0.2 某些效果依赖阵列图案，则把所有格子视为像素，但仅有灯的61个格子能显示出来，其它的忽略
//
// TODO:
//   . 制定通信协议
//          - 指令式，不用返回状态，也没有查询指令
//      =====. ✓协议格式：6.21确定方案： 使用JSON，判定全部使用String类
//      ====>. 编码：协议格式
//      . 编码：串口接收协议消息，并进行解析，调用对应的处理函数，协议用通用指令，包括：
//                  * 切换模式
//                  * 修改指定模式的参数   // 如果是当前模式，则会调用一次本模式的初始化函数
//      . 编码：每个模式，包括两个函数：初始化、Update，协议命令修改参数配置后，会再调用一次初始化函数
//      . 编码：实现ESP TCP Server
//      . 编码：用手机APP发送TCP消息给ESP
//      . 以上在周一上班期间，全部编码完成，晚上回家调试
//#define MSG_HEAD        0x5F    // 二进制为：0101 1111  字符为： _
//#define MSG_TERMINATOR  0x5E    // 二进制为：0101 1110  字符为： ^
//
//// .0 模式常量：代表当前的效果模式
//#define MODE_COUNT              9
//#define MODE_DEFAULT_Breathing  0     // 预设：基本-呼吸灯
//#define MODE_DEFAULT_Mosaic     1     // 预设：基本-马赛克
//#define MODE_DEFAULT_Ambilight  2     // 预设：基本-流光
//#define MODE_VOCAL_Histogram    3     // 人声：基本-柱状变化
//#define MODE_VOCAL_Brightness   4     // 人声：基本-亮度变化
//#define MODE_VOCAL_Concentric   5     // 人声：基本-同心圆
//#define MODE_MUSIC_Ease         6     // 音乐：基本-和缓
//#define MODE_MUSIC_Cheerful     7     // 音乐：基本-欢快
//#define MODE_MUSIC_Passionate   8     // 音乐：基本-激情
//
//#define DEFAUTL_ROW_NUM         12
//#define DEFAULT_COL_NUM         23
//
////================================================
//// 系统配置，下面的数据都将保存在EEPROM中
//// .灯带组合形式
//typedef struct __LedGridConfig
//{
//  uint8_t   led_grid_count;     // 有效格子的总数，默认60
//  uint8_t   row_num;            // 总行数，默认12
//  uint8_t   col_num;            // 总列数，默认23
//  uint8_t   ic_num_per_grid;    // 每格子中的芯片数，默认8
//} LedGridConfig;
//
//// 用于保存模式的参数值，每参数可以保存5个值
//#define PARAM_NUM     5
//typedef struct __ModeParams
//{
//  uint8_t   raw[PARAM_NUM];           // 字节参数，用于保存可用字节表示的非线性配置项
//  CRGB      color[PARAM_NUM];         // 色彩项
//  float     speed[PARAM_NUM];         // 速度项
//  float     amplitude[PARAM_NUM];     // 幅度项
//} ModeParams;
//
//const uint8_t SAVED_FLAG = 0x35;
//typedef struct __Config
//{
//  LedGridConfig   led_grid_config;
//  ModeParams      mode_params[MODE_COUNT];
//} Config;
//
//// 数据线所在PIN
//#define DATA_PIN               23

// . 全局变量：
//uint8_t   g_current_mode  = 0;  // 当前模式
//Config    g_config;             // 配置表
//CRGB*     gp_leds;              // LED对象数组


//
// 初始化
//
void setup()
{
  // 0.串口
  Serial.begin(115200);

  // 1.读取配置
  size_t size_of_config = GetConfigNeedBytes();
  size_t size_of_matrix = GetMatrixNeedBytes();
  
  Serial.println("Size of EEPROM storage space required:");
  Serial.print("\tSize of config: "); Serial.println(size_of_config);
  Serial.print("\tSize of matrix: "); Serial.println(size_of_matrix);

//  EEPROM.begin(1 + sizeof(g_mode_params));
//  setup_read_configs();
//  setup_read_run_params();
//  debug_print_config_and_params();  // 打印所有值
//
//  // 2.LED驱动
//  Serial.println("Init LED driver...");
//  FastLED.addLeds<UCS1903, DATA_PIN, BGR>(g_leds, LEDS_NUM);
//  LEDS.setBrightness(96);
//  Serial.println("LED driver OK.");
//  delay(500);
//  LEDS.setBrightness(0);

  // 3.拾音器

  // 4.其它
}

void loop()
{
//  // 1. 检查串口消息
//  if (Serial.available())
//  {
//    receive_and_process_message_from_serial();
//  }
//
//  // 2. 检查网络消息
//
//  // 3. 处理最新指令
//
//  // 4. 根据当前配置，运行指定的模式
//  switch (g_current_mode)
//  {
//    // 0: 默认： 呼吸灯
//    case MODE_DEFAULT_Breathing:
//      {
//        if (g_last_mode != g_current_mode)
//        {
//          Initialize_Mode_Default_Breathing();
//          g_last_mode = g_current_mode;
//        }
//
//        Process_Mode_Default_Breathing();
//      }
//      break;
//    default:
//      {
//        // 配置错误，默认恢复到呼吸灯
//        g_current_mode = MODE_DEFAULT_Breathing;
//      }
//      break;
//  }
//
//  LEDS.show();
//  delay(10);
}
//
////---------------------------------------------------
//// 命令消息处理函数区
//// 目前支持：
////    串口：从标准串口接收消息
////    Wifi client：本机做为Wifi Station，由其它设备用特定SSID和密码连接上来，用TCP发送命令消息
//// 后续可能支持：
////    以太网：加入已有的局域网，由其它局域网中的设备发送命令指令过来
////    Wifi局域网：加入已有的局域网，由其它局域网中的设备发送命令指令过来
//
////---------------------------------------------------
//// 接收串口消息
//void receive_and_process_message_from_serial()
//{
//  int incomingByte = Serial.read();
//  if (incomingByte == MSG_HEAD)
//  {
//    String msg = Serial.readStringUntil(MSG_TERMINATOR);
//    Serial.print("Received msg:");
//    Serial.println(msg);
//    if (msg.length() > 1)
//    {
//      // 直接处理消息
//      process_message(msg);
//    }
//  }
//}
//
////---------------------------------------------------
//// 处理消息
//void process_message(String& msg)
//{
//  DynamicJsonDocument doc(1024);
//  DeserializationError error = deserializeJson(doc, msg);
//  if (error)
//  {
//    Serial.print("deserializeMsgPack() failed: ");
//    Serial.println(error.c_str());
//    return;
//  }
//
//  JsonObject root = doc.as<JsonObject>();
//
//  int use_mode = g_current_mode;
//
//  for (JsonPair kv : root)
//  {
//    String key = kv.key().c_str();
//    if (key == "mode")
//    {
//      use_mode = kv.value().as<int>();
//      Serial.print("will set mode to :"); Serial.println(use_mode);
//    }
//    else if (key == "row")
//    {
//      Serial.print("row.is<JsonArray>() = "); Serial.println(kv.value().is<JsonArray>());
//      Serial.print("row.is<JsonObject>() = "); Serial.println(kv.value().is<JsonObject>());
//    }
//    else if (key == "color")
//    {
//      JsonObject color = kv.value().as<JsonObject>();
//      for (JsonPair c_kv : color)
//      {
//        Serial.print(c_kv.key().c_str()); Serial.println(c_kv.value().as<String>());
//
//      }
//
//    }
//    else if (key == "speed")
//    {
//      Serial.print("speed.is<JsonArray>() = "); Serial.println(kv.value().is<JsonArray>());
//      Serial.print("speed.is<JsonObject>() = "); Serial.println(kv.value().is<JsonObject>());
//
//    }
//    else if (key == "amplitude")
//    {
//      Serial.print("amplitude.is<JsonArray>() = "); Serial.println(kv.value().is<JsonArray>());
//      Serial.print("amplitude.is<JsonObject>() = "); Serial.println(kv.value().is<JsonObject>());
//
//    }
//  }
//
//}
//
////==================================================
//// 模式效果处理函数区
////   公用变更
//unsigned long mode_start_millis = 0;
//
////==================================================
//// 模式： 0： 默认： 基础-呼吸灯
////   参数意义说明：
////      速度0：0~30，值=0时：停止； 值>0时，为每个呼吸循环的秒数，最大值为30
////      幅度0：最低亮度，0~1，硬件值为 255 * a
////      幅度1：最高亮度，0~1，硬件值为 255 * a
//unsigned long millis_per_cycle            = 0; // 每循环用millis
//unsigned long millis_current_cycle_start  = 0; // 本循环的起始millis
//uint8_t brightness_min = 1;
//uint8_t brightness_max = 255;
//void Initialize_Mode_Default_Breathing()
//{
//  // 0. 计算参数
//  if (g_mode_params[MODE_DEFAULT_Breathing].speed[0] < 0)
//    millis_per_cycle = 0;
//  else if (g_mode_params[MODE_DEFAULT_Breathing].speed[0] > 30)
//    millis_per_cycle = 1000 * 30;
//  else
//    millis_per_cycle = 1000 * g_mode_params[MODE_DEFAULT_Breathing].speed[0];
//
//  if (g_mode_params[MODE_DEFAULT_Breathing].amplitude[0] < 0 || g_mode_params[MODE_DEFAULT_Breathing].amplitude[0] > 1)
//    brightness_min = 0;
//  else
//    brightness_min = 255 * g_mode_params[MODE_DEFAULT_Breathing].amplitude[0];
//
//  if (g_mode_params[MODE_DEFAULT_Breathing].amplitude[1] < 0 || g_mode_params[MODE_DEFAULT_Breathing].amplitude[1] > 1)
//    brightness_max = 255;
//  else
//    brightness_max = 255 * g_mode_params[MODE_DEFAULT_Breathing].amplitude[1];
//
//  // 设置颜色
//  for (int i = 0; i < LEDS_NUM; i++)
//  {
//    g_leds[i] = g_mode_params[0].color[0];
//  }
//
//  // 本模式的启动时间
//  mode_start_millis = millis();
//
//  // 本呼吸循环的启动时间（如果是在循环期间修改的参数，则不重置本循环启动时间）
//  if (mode_start_millis - millis_current_cycle_start > millis_per_cycle)
//  {
//    millis_current_cycle_start = mode_start_millis;
//  }
//}
//
//void Process_Mode_Default_Breathing()
//{
//  // 按速度和幅度配置循环
//  unsigned long current_millis = millis();
//  unsigned long delta = current_millis - millis_current_cycle_start;
//  if (delta > millis_per_cycle)
//  {
//    millis_current_cycle_start = current_millis;
//    delta = 0;
//  }
//
//  static unsigned long millis_per_half_cycle = millis_per_cycle / 2;
//  unsigned long lerp_point = delta;
//  // 前半段从暗变亮
//  if (delta > millis_per_half_cycle)
//  {
//    lerp_point = millis_per_cycle - delta;
//  }
//
//  int bright = map(lerp_point, 0, millis_per_half_cycle, brightness_min, brightness_max);
//  LEDS.setBrightness(bright);
//}
