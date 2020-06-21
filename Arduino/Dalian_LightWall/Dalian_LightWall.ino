#include <EEPROM.h>
#include <FastLED.h>

//
// 大连LED灯墙项目
// 0.概述：
//    0.1 此灯墙为12行23列的格子组成，但仅其中的61个格子中有灯光
//    0.2 某些效果依赖阵列图案，则把所有格子视为像素，但仅有灯的61个格子能显示出来，其它的忽略
//

// 1.灯带的裁剪和安装规格
#define GRID_COUNT       61    // 格子总数
#define GRID_ROW_NUM     12    // 行数
#define GRID_IC_NUM       8    // 每格子芯片数

// 2.格子阵列，其中，0代表此格子无灯，其它代表了它在LED串联中的起始芯片索引
//   格子与墙面上的排列是倒映的，即第00行对应墙上最下面的那一行，这样方便计算和更新
const uint8_t WallGridMatrix[12][23] =
{
  // TODO: 先做呼吸灯，不用这个，等确认灯数后再填写
  //00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 // 列号 第00行是最靠下的
  { 00, 00, 00, 00, 00, 00, 00, 01, 00, 02, 00, 00, 00, 00, 00, 00, 00, 00, 03, 00, 00, 00, 00 }, // 第00行
  { 00, 00, 00, 00, 04, 05, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第01行
  { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第02行
  { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第03行
  { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第04行
  { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第05行
  { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第06行
  { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第07行
  { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第08行
  { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第09行
  { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第10行
  { 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00 }, // 第11行
};

// 3.实际的芯片数和硬件相关定义
//  3.1 芯片总数
const uint16_t LEDS_NUM = (GRID_IC_NUM * GRID_COUNT);
CRGB g_leds[LEDS_NUM];
//  3.2 数据线所在PIN
#define DATA_PIN               23

// 4.状态信息和可配置项
//  4.0 模式常量
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

//  4.1 当前模式
uint8_t g_current_mode  = 0;
uint8_t g_last_mode = 255;

//  4.2 当前模式的参数值
typedef struct __ModeParams
{
  uint8_t raw[5];       // 字节参数，用于保存可用字节表示的非线性配置项
  CRGB color[5];        // 色彩项
  float speed[5];       // 速度项
  float amplitude[5];   // 幅度项
} ModeParams;

const uint8_t SAVED_FLAG = 0x35;
ModeParams g_mode_params[MODE_COUNT];

//
// 初始化
//
void setup()
{
  // 0.串口
  Serial.begin(115200);

  // 1.读取配置
  Serial.print("Size of ModeParams:");
  Serial.println(sizeof(ModeParams));
  Serial.print("Size of g_mode_params:");
  Serial.println(sizeof(g_mode_params));
  Serial.println("Reading config params from EEPROM...");
  EEPROM.begin(1 + sizeof(g_mode_params));
  Setup_ReadParams();         // 从 flash 中读取
  Debug_PrintConfigParams();  // 打印所有值

  // 2.LED驱动
  Serial.println("Init LED driver...");
  FastLED.addLeds<UCS1903, DATA_PIN, BGR>(g_leds, LEDS_NUM);
  LEDS.setBrightness(96);
  Serial.println("LED driver OK.");
  delay(500);
  LEDS.setBrightness(0);

  // 3.拾音器

  // 4.其它
}

void loop()
{
  // 1. 检查串口消息

  // 2. 检查网络消息

  // 3. 处理最新指令

  // 4. 根据当前配置，运行指定的模式
  switch (g_current_mode)
  {
    // 0: 默认： 呼吸灯
    case MODE_DEFAULT_Breathing:
      {
        if (g_last_mode != g_current_mode)
        {
          Initialize_Mode_Default_Breathing();
          g_last_mode = g_current_mode;
        }
        
        Process_Mode_Default_Breathing();
      }
      break;
    default:
      {
        // 配置错误，默认恢复到呼吸灯
        g_current_mode = MODE_DEFAULT_Breathing;
      }
      break;
  }

  LEDS.show();
  delay(10);
}

///////////////////////////////////////////////////////////////
// 将配置恢复为默认值
void Setup_ResetParamsToDefault()
{
  //  uint8_t raw[5];
  //  CRGB color[5];
  //  float speed[5];
  //  float amplitude[5];
  // 1. 全部置0
  memset(g_mode_params, 0, sizeof(g_mode_params));

  // 2. 将特定的参数修改为需要的值
  //  2.1 呼吸模式
  g_mode_params[0].raw[0]       = 0;            // 呼吸模式：简单模式
  g_mode_params[0].color[0]     = CRGB::White;  // 颜色：白色
  g_mode_params[0].speed[0]     = 12;          // 速度：中速
  g_mode_params[0].amplitude[0] = 0.0;          // 幅度底：最小
  g_mode_params[0].amplitude[0] = 1.0;          // 幅度峰：最大
}


///////////////////////////////////////////////////////////////
// 读取配置
void Setup_ReadParams()
{
  // 1. 读取第0字节，判定是否存储过配置，如果没有存过，则立刻写入默认值
  if (EEPROM.read(0) != SAVED_FLAG)
  {
    Serial.println("Save default confgi PARAMS to FLASH...");
    Setup_ResetParamsToDefault();
    Setup_SaveParams();
  }
  else
  {
    // 2. 正常读取 FLASH 中的内容
    for (int i = 0; i < MODE_COUNT; i++)
    {
      EEPROM.get(sizeof(SAVED_FLAG) + i * sizeof(ModeParams), g_mode_params[i]);
    }
  }
}

///////////////////////////////////////////////////////////////
// 写入配置
void Setup_SaveParams()
{
  EEPROM.write(0, SAVED_FLAG);
  for (int i = 0; i < MODE_COUNT; i++)
  {
    EEPROM.put(sizeof(SAVED_FLAG) + i * sizeof(ModeParams), g_mode_params[i]);
  }
  EEPROM.commit();
}

///////////////////////////////////////////////////////////////
// 打印配置
void Debug_PrintConfigParams()
{
  Serial.println("Current Config Params Values:");
  for (int i = 0; i < MODE_COUNT; i++)
  {
    Serial.print("MODE "); Serial.println(i);
    Serial.print("---- raw = ");
    for (int x = 0; x < 5; x++)
    {
      Serial.print(g_mode_params[i].raw[x]);
      Serial.print("\t");
    }
    Serial.println("<<end.");

    Serial.print("----  color = ");
    for (int x = 0; x < 5; x++)
    {
      Serial.print("0x");
      Serial.print(g_mode_params[i].color[x], HEX);
      Serial.print("\t");
    }
    Serial.println("<<end.");

    Serial.print("----  speed = ");
    for (int x = 0; x < 5; x++)
    {
      Serial.print(g_mode_params[i].speed[x], 2);
      Serial.print("\t");
    }
    Serial.println("<<end.");

    Serial.print("----  amplitude = ");
    for (int x = 0; x < 5; x++)
    {
      Serial.print(g_mode_params[i].amplitude[x], 2);
      Serial.print("\t");
    }
    Serial.println("<<end.");
    Serial.println("---------------------------");
  }
}

//==================================================
// 模式效果处理函数区
//   公用变更
unsigned long mode_start_millis = 0;

//==================================================
// 模式： 0： 默认： 基础-呼吸灯
//   参数意义说明：
//      速度0：0~30，值=0时：停止； 值>0时，为每个呼吸循环的秒数，最大值为30
//      幅度0：最低亮度，0~1，硬件值为 255 * a
//      幅度1：最高亮度，0~1，硬件值为 255 * a
unsigned long millis_per_cycle            = 0; // 每循环用millis
unsigned long millis_current_cycle_start  = 0; // 本循环的起始millis
uint8_t brightness_min = 1;
uint8_t brightness_max = 255;
void Initialize_Mode_Default_Breathing()
{
  // 0. 计算参数
  if (g_mode_params[MODE_DEFAULT_Breathing].speed[0] < 0)
    millis_per_cycle = 0;
  else if (g_mode_params[MODE_DEFAULT_Breathing].speed[0] > 30)
    millis_per_cycle = 1000 * 30;
  else
    millis_per_cycle = 1000 * g_mode_params[MODE_DEFAULT_Breathing].speed[0];

  if (g_mode_params[MODE_DEFAULT_Breathing].amplitude[0] < 0 || g_mode_params[MODE_DEFAULT_Breathing].amplitude[0] > 1)
    brightness_min = 0;
  else
    brightness_min = 255 * g_mode_params[MODE_DEFAULT_Breathing].amplitude[0];

  if (g_mode_params[MODE_DEFAULT_Breathing].amplitude[1] < 0 || g_mode_params[MODE_DEFAULT_Breathing].amplitude[1] > 1)
    brightness_max = 255;
  else
    brightness_max = 255 * g_mode_params[MODE_DEFAULT_Breathing].amplitude[1];
    
  // 设置颜色
  for (int i = 0; i < LEDS_NUM; i++)
  {
    g_leds[i] = g_mode_params[0].color[0];
  }

  // 本模式的启动时间
  mode_start_millis = millis();

  // 本呼吸循环的启动时间（如果是在循环期间修改的参数，则不重置本循环启动时间）
  if (mode_start_millis - millis_current_cycle_start > millis_per_cycle)
  {
    millis_current_cycle_start = mode_start_millis;
  }
}

void Process_Mode_Default_Breathing()
{
  // 按速度和幅度配置循环
  unsigned long current_millis = millis();
  unsigned long delta = current_millis - millis_current_cycle_start;
  if (delta > millis_per_cycle)
  {
    millis_current_cycle_start = current_millis;
    delta = 0;
  }

  static unsigned long millis_per_half_cycle = millis_per_cycle / 2;
  unsigned long lerp_point = delta;
  // 前半段从暗变亮
  if (delta > millis_per_half_cycle)
  {
    lerp_point = millis_per_cycle - delta;
  }

  int bright = map(lerp_point, 0, millis_per_half_cycle, brightness_min, brightness_max);
  LEDS.setBrightness(bright);
}
