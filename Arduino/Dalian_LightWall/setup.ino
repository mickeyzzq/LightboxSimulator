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
void setup_read_led_configs()
{

}

// 读取效果参数
void setup_read_run_params()
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
void debug_print_config_and_params()
{
  Serial.println("Current Config Params Values:");
  for (int i = 0; i < MODE_COUNT; i++)
  {
    Serial.print("MODE "); Serial.println(i);
    Serial.print("----  raw = ");
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
