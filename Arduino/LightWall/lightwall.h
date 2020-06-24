#include <EEPROM.h>
#include <ArduinoJson.h>
#include <FastLED.h>

#define MSG_HEAD        0x5F    // 二进制为：0101 1111  字符为： _
#define MSG_TERMINATOR  0x5E    // 二进制为：0101 1110  字符为： ^

// .0 模式常量：代表当前的效果模式
#define MODE_COUNT              9
#define MODE_DEFAULT_Breathing  0     // 预设：基本-呼吸灯
#define MODE_DEFAULT_Mosaic     1     // 预设：基本-马赛克
#define MODE_DEFAULT_Ambilight  2     // 预设：基本-流光
#define MODE_VOCAL_Histogram    3     // 人声：基本-柱状变化
#define MODE_VOCAL_Brightness   4     // 人声：基本-亮度变化
#define MODE_VOCAL_Concentric   5     // 人声：基本-同心圆
#define MODE_MUSIC_Ease         6     // 音乐：基本-和缓
#define MODE_MUSIC_Cheerful     7     // 音乐：基本-欢快
#define MODE_MUSIC_Passionate   8     // 音乐：基本-激情

#define DEFAUTL_ROW_NUM         12
#define DEFAULT_COL_NUM         23

//================================================
// 系统配置，下面的数据都将保存在EEPROM中
// .灯带组合形式
typedef struct __LedGridConfig
{
  uint8_t   led_grid_count;     // 有效格子的总数，默认60
  uint8_t   ic_num_per_grid;    // 每格子中的芯片数，默认8
  uint8_t   row_num;            // 总行数，默认12
  uint8_t   col_num;            // 总列数，默认23
} LedGridConfig;

// 用于保存模式的参数值，每参数可以保存5个值
#define PARAM_NUM     5
typedef struct __ModeParams
{
  uint8_t   raw[PARAM_NUM];           // 字节参数，用于保存可用字节表示的非线性配置项
  float     speed[PARAM_NUM];         // 速度项
  float     amplitude[PARAM_NUM];     // 幅度项
  CRGB      color[PARAM_NUM];         // 色彩项
} ModeParams;

typedef struct __Config
{
  uint8_t         current_mode;
  LedGridConfig   led_grid_config;
  ModeParams      mode_params[MODE_COUNT];
} Config;

// 存储是否初始化的标记
#define SAVED_FLAG      0x35

// 数据线所在PIN
#define DATA_PIN               23

// . 全局变量：
extern Config         g_config;                   // 配置表
extern unsigned int   g_ledsNum;                  // LED芯片数量
extern CRGB*          gp_leds;                    // LED对象数组
extern bool           g_needReinitializeMode;     // 是否需要对当前模式重新初始化
extern unsigned long  g_mode_start_millis;        // 模式的启动时间
extern bool           g_print_mode_info;
extern unsigned int   g_print_mode_stoptime;

// 函数声明
// Version.cpp
void PrintVersionInfo();

// Config.cpp
size_t GetConfigNeedBytes();
size_t GetMatrixNeedBytes();
bool IsNewDevice();
void ResetConfigToDefault();
void SaveConfig();
void ReadConfig();
void PrintConfig();
void PrintStoregeInfo();

// SerialMessage.cpp
void ReceiveAndProcessSerialMessage();
void ReceiveAndProcessATCommand();

// Message.cpp
void ProcessMessage(String& msg);

// Mode 0 : 呼吸灯
void Mode_0_Initialize();
void Mode_0_Process();