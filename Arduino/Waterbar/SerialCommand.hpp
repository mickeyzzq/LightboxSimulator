#ifndef _SERIALCOMMAND_HPP_
#define _SERIALCOMMAND_HPP_

// 串口指令的消息头和尾
#define MSG_HEAD        0x5F    // 二进制为：0101 1111  字符为： _
#define MSG_TERMINATOR  0x5E    // 二进制为：0101 1110  字符为： ^

// 串口指令接收
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
      ProcessCommandMessage(msg);
    }
  }
}

///////////////////////////////////////
// AT Command
void ReceiveAndProcessATCommand()
{
  static int old_mode = 1;

  String cmd = Serial.readString();
  if (cmd == "p ver")                   // 打印版本信息
  {
    PrintVersionInfo();
  }
  else if (cmd == "p allcfg")           // 打印所有配置信息
  {
    ConfigManager.PrintConfig();
  }
  else if (cmd == "p cfg")              // 打印基础配置
  {
    ConfigManager.PrintConfigValues();
  }
  else if (cmd == "p grid")
  {
    ConfigManager.PrintGridValues();
  }

  else if (cmd == "rst factory")        // 恢复出厂设置
  {
    Serial.println("Reset config to Factory DEFAULT!");
    ConfigManager.ResetToFactorySettings();

    Mode_Test::DefaultConfig(ConfigManager.config.mode[0]);
    Mode_Breath::DefaultConfig(ConfigManager.config.mode[1]);
    ConfigManager.Save();
    ConfigManager.modeChanged = true;
  }

  else if (cmd == "test run")
  {
    Serial.println("Test mode running...");
    old_mode = ConfigManager.config.current_mode;
    ConfigManager.config.current_mode = 0;
    ConfigManager.modeChanged = true;
  }
  else if (cmd == "test stop")
  {
    ConfigManager.config.current_mode = old_mode;
    ConfigManager.modeChanged = true;
    Serial.print("Test mode stopped, restore to mode "); Serial.println(old_mode);
  }
  //  else if (cmd == "ps")               // 打印存储信息
  //  {
  //    PrintStoregeInfo();
  //  }
  //  else if (cmd == "pd")   // 打印当前模式的运行时调试信息
  //  {
  //    g_print_mode_info = true;
  //    g_print_mode_stoptime = 3000 + millis();
  //    Serial.print("Current mode:"); Serial.print(g_config.current_mode);
  //    Serial.println("Runtime info:");
  //  }
  //  else if (cmd == "pg")   // 打印全局变量表
  //  {
  //
  //  }
  //  else if (cmd == "mi")   // 当前模式初始化
  //  {
  //    g_needReinitializeMode = true;
  //  }
}

#endif
