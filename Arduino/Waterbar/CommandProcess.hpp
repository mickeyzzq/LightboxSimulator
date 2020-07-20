#ifndef _COMMANDPROCESS_HPP_
#define _COMMANDPROCESS_HPP_

#include <ArduinoJson.h>

extern Mode* modes[];

void ProcessCommandMessage(String& msg)
{
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, msg);
  if (error)
  {
    Serial.print("deserializeMsgPack() failed: ");
    Serial.println(error.c_str());
    return;
  }

  JsonObject root = doc.as<JsonObject>();
  int use_mode = ConfigManager.config.current_mode;   // 取当前模式，如果指令中不指定mode，则认为是设置当前模式下的参数值
  struct Config::Mode newMode;

  struct DirtyFlags isDirty = { false, false, false, false, false };

  for (JsonPair kv : root)
  {
    String key = kv.key().c_str();

    if (key == "mode")                                // 指定了模式，如不同则需要切换
    {
      use_mode = kv.value().as<int>();
      Serial.print("Command: Change mode to :"); Serial.println(use_mode);

      //isConfigDirty = true;
    }
    else if (key == "flag")
    {
      Serial.print("Command: Modify config param [flag] in Mode:"); Serial.println(use_mode);
      newMode.flag = kv.value().as<bool>();
      Serial.print(". new value = "); Serial.println(newMode.flag);
      isDirty.flag = true;
    }
    else if (key == "brightest")
    {
      Serial.print("Command: Modify config param [brightest] in Mode:"); Serial.println(use_mode);
      newMode.brightest = kv.value().as<float>();
      Serial.print(". new value = "); Serial.println(newMode.brightest);
      isDirty.brightest = true;
    }
    else if (key == "darkest")
    {
      Serial.print("Command: Modify config param [darkest] in Mode:"); Serial.println(use_mode);
      newMode.darkest = kv.value().as<float>();
      Serial.print(". new value = "); Serial.println(newMode.darkest);
      isDirty.darkest = true;
    }
    else if (key == "speed")
    {
      Serial.print("Command: Modify config param [speed] in Mode:"); Serial.println(use_mode);
      newMode.speed = kv.value().as<float>();
      Serial.print(". new value = "); Serial.println(newMode.speed);
      isDirty.speed = true;
    }
    else if (key == "color")
    {
      Serial.print("Command: Modify config param [color] in Mode:"); Serial.println(use_mode);
      String color_val_hex = kv.value().as<String>();
      newMode.color = (int) strtol( color_val_hex.c_str(), NULL, 16);
      Serial.print(". new value = 0x"); Serial.println(newMode.color);
      isDirty.color = true;
    }
  }

  Config::Mode& useMode = ConfigManager.config.mode[use_mode];

  if (isDirty.flag) useMode.flag = newMode.flag;
  if (isDirty.brightest) useMode.brightest = newMode.brightest;
  if (isDirty.darkest) useMode.darkest = newMode.darkest;
  if (isDirty.speed) useMode.speed = newMode.speed;
  if (isDirty.color) useMode.color = newMode.color;

  if (use_mode != ConfigManager.config.current_mode || isDirty)
  {
    if (use_mode != ConfigManager.config.current_mode)
    {
      ConfigManager.config.current_mode = use_mode;
      ConfigManager.modeChanged = true;      
    }

    ConfigManager.Save();

    if (modes[use_mode] != NULL)
    {
      modes[use_mode]->OnConfigChanged(isDirty);
    }
  }
}
#endif
