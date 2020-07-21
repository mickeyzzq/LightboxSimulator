//////////////////////////////////////////////////////////////////////
// 大连某售楼大厅水吧台音乐灯光背景墙项目
//
//   项目信息：
//     施工：胡日荣 18675570761
//     地址：辽宁省大连市甘井子区
//     灯带型号： WS2811 12V 每IC三灯5cm
//     安装方式：每格一个单元，每单元30cm * 2 = 60cm，共12片IC
//              因柱子问题，导致安装顺序与视觉顺序不同，需要程序进行修正
//////////////////////////////////////////////////////////////////////

#include "waterbar.h"
#define DEBUG   true
//#define RELEASE false

Mode* modes[12];
int current_mode;
bool g_pause = false;

void setup()
{
  // Setup步骤：
  // .初始化串口
  // .打印版本信息
  // .读取EPROM中的配置信息
  //    .需要重置为出厂设置？
  //      Y.重置为出厂设置
  // .  .打印配置表（DEBUG）
  // .初始化麦克风
  // .初始化ADC的AUX输入
  // .初始化LED阵列
  // .  .根据配置，创建LED阵列

  // 初始化串口
  Serial.begin(115200);

  // 输出版权和版本信息
  PrintVersionInfo();

  // 读取EPROM中的配置信息
  ConfigManager.ReadFromEEPROM();

  current_mode = ConfigManager.config.current_mode;

  // 输出配置信息
  //ConfigManager.PrintConfig();

  //TODO: 初始化ADC输入（AUX）

  //TODO: 初始化麦克风输入（INMP441）

  // 初始化Wifi
  UDPServer.Begin();

  // 初始化LED灯组
  Grids.InitializeLEDArray();
  // 创建各个模式
  modes[0] = new Mode_Test(0);             // 测试模式
  modes[1] = new Mode_Breath(1);
  modes[2] = new Mode_Mosaic(2);
  modes[3] = new Mode_AudioVis(3);
  modes[4] = new Mode_AudioScroll(4);
  // modes[5] = new MusicMode_Light;
  // modes[6] = new MusicMode_Histogram;
  // modes[7] = new NoiceMode_Simple;
  // modes[8] = new NoiceMode_Histogram;
}

void loop()
{
  // 主循环步骤：
  // .检查Wifi状态
  // .尝试接收和处理串口指令，直到清空缓冲区
  // .尝试接收和处理TCP指令，直到清空缓冲区
  // .运行当前模式
  //    .是否需要初始化当前模式
  //      Y.初始化
  //      N.忽略
  //    .运行模式主代码
  // .延时
  // .LEDS.show

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
  UDPServer.Process();

  // . 运行模式
  Mode* pCurMode = modes[current_mode];
  if (ConfigManager.modeChanged || pCurMode->NeedInitialize())
  {
    current_mode = ConfigManager.config.current_mode;
    pCurMode = modes[current_mode];
    pCurMode->Initialize();

    ConfigManager.modeChanged = false;
  }

  if (!g_pause)
  {
    if (pCurMode->Process())
    {
      Grids.Show();
    }
    else
    {
      delay(30);
    }
  }
  else
  {
    delay(100);
  }
}
