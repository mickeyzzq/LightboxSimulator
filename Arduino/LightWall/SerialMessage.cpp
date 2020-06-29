#include <Arduino.h>
#include "lightwall.h"

void ReceiveAndProcessSerialMessage()
{
  int incomingByte = Serial.read();
  if (incomingByte == MSG_HEAD)
  {
    String msg = Serial.readStringUntil(MSG_TERMINATOR);
    Serial.print("Serial msg:"); Serial.println(msg);
    if (msg.length() > 1)
    {
      // 直接处理消息
      ProcessMessage(msg);
    }
  }
}

///////////////////////////////////////
// AT Command
// pv  - print version info
// pc  - print config values
// ps  - print storge info
void ReceiveAndProcessATCommand()
{
  String cmd = Serial.readString();
  if (cmd == "pv")        // 打印版本信息
  {
    PrintVersionInfo();
  }
  else if (cmd == "pc")   // 打印配置表
  {
    PrintConfig();
  }
  else if (cmd == "ps")   // 打印存储信息
  {
    PrintStoregeInfo();
  }
  else if (cmd == "pd")   // 打印当前模式的运行时调试信息
  {
    g_print_mode_info = true;
    g_print_mode_stoptime = 3000 + millis();
    Serial.print("Current mode:"); Serial.print(g_config.current_mode);
    Serial.println("Runtime info:");
  }
  else if (cmd == "pg")   // 打印全局变量表
  {
g_ledsNum;
g_needReinitializeMode;
g_mode_start_millis;        // 模式的启动时间
g_print_mode_info;
g_print_mode_stoptime;
  }
  else if (cmd == "mi")   // 当前模式初始化
  {
    g_needReinitializeMode = true;
  }
  else if (cmd == "reset")   // 当前模式初始化
  {
    Serial.println("Reset config to Factory DEFAULT!");
    ResetConfigToDefault();
    SaveConfig();
  }
}
