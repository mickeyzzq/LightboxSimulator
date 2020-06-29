#include <Arduino.h>
#include "lightwall.h"

void ProcessMessage(String& msg)
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
  int use_mode = g_config.current_mode;
  bool isConfigDirty = false;

  for (JsonPair kv : root)
  {
    String key = kv.key().c_str();

    if (key == "mode")
    {
      use_mode = kv.value().as<int>();
      Serial.print("Command: Change mode to :"); Serial.println(use_mode);
      g_needReinitializeMode = (g_config.current_mode != use_mode);
      g_config.current_mode = use_mode;
      isConfigDirty = true;
    }
    else if (key == "row")
    {
      Serial.print("Command: Modify config param [row] in Mode:"); Serial.println(use_mode);
      JsonObject row = kv.value().as<JsonObject>();
      for (JsonPair row_kv : row)
      {
        const char* str_row_key = row_kv.key().c_str();
        if (isDigit(str_row_key[0]))
        {
          int row_idx = atoi(str_row_key);
          uint8_t row_val = row_kv.value().as<uint8_t>();
          Serial.print("\tSet row["); Serial.print(row_idx); Serial.print("] to :");
          Serial.print(row_val); Serial.println(".");
          g_config.mode_params[use_mode].raw[row_idx] = row_val;
          isConfigDirty = true;
        }
      }
    }
    else if (key == "color")
    {
      Serial.print("Command: Modify config param [color] in Mode:"); Serial.println(use_mode);
      JsonObject color = kv.value().as<JsonObject>();
      for (JsonPair color_kv : color)
      {
        const char* str_color_key = color_kv.key().c_str();
        if (isDigit(str_color_key[0]))
        {
          int color_idx = atoi(str_color_key);
          String color_val_hex = color_kv.value().as<String>();
          int color_val = (int) strtol( color_val_hex.c_str(), NULL, 16);
          Serial.print("\tSet color["); Serial.print(color_idx); Serial.print("] to :0x");
          Serial.print(color_val, HEX); Serial.print("("); Serial.print(color_val_hex); Serial.println(").");
          g_config.mode_params[use_mode].color[color_idx] = color_val;
          isConfigDirty = true;
        }
      }
    }
    else if (key == "speed")
    {
      Serial.print("Command: Modify config param [speed] in Mode:"); Serial.println(use_mode);
      JsonObject speed_obj = kv.value().as<JsonObject>();
      for (JsonPair speed_kv : speed_obj)
      {
        const char* str_speed_key = speed_kv.key().c_str();
        if (isDigit(str_speed_key[0]))
        {
          int speed_idx = atoi(str_speed_key);
          float speed_val = speed_kv.value().as<float>();
          Serial.print("\tSet speed["); Serial.print(speed_idx); Serial.print("] to :");
          Serial.print(speed_val); Serial.println(".");
          g_config.mode_params[use_mode].speed[speed_idx] = speed_val;
          isConfigDirty = true;
        }
      }
    }
    else if (key == "amplitude")
    {
      Serial.print("Command: Modify config param [amplitude] in Mode:"); Serial.println(use_mode);
      JsonObject amplitude = kv.value().as<JsonObject>();
      for (JsonPair amplitude_kv : amplitude)
      {
        const char* str_amplitude_key = amplitude_kv.key().c_str();
        if (isDigit(str_amplitude_key[0]))
        {
          int amplitude_idx = atoi(str_amplitude_key);
          float amplitude_val = amplitude_kv.value().as<float>();
          Serial.print("\tSet amplitude["); Serial.print(amplitude_idx); Serial.print("] to :");
          Serial.print(amplitude_val); Serial.println(".");
          g_config.mode_params[use_mode].amplitude[amplitude_idx] = amplitude_val;
          isConfigDirty = true;
        }
      }
    }
    else if (key == "grid_count")
    {
      g_config.led_grid_config.led_grid_count   = kv.value().as<uint8_t>();
      isConfigDirty = true;
    }
    else if (key == "num_per_grid")
    {
      g_config.led_grid_config.ic_num_per_grid  = kv.value().as<uint8_t>();
      isConfigDirty = true;
    }
    else if (key == "row_num")
    {
      g_config.led_grid_config.row_num          = kv.value().as<uint8_t>();
      isConfigDirty = true;
    }
    else if (key == "col_num")
    {
      g_config.led_grid_config.col_num          = kv.value().as<uint8_t>();
      isConfigDirty = true;
    }
  }

  if (isConfigDirty)
  {
    SaveConfig();
    isConfigDirty = false;
  }
}
