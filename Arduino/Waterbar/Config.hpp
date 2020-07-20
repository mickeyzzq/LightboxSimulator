#ifndef _CONFIG_HPP_
#define _CONFIG_HPP_

#define DEFAUTL_ROW_NUM 12
#define DEFAULT_COL_NUM 23

//#define CONFIG_FLAG     0x37

#define MODE_NUM        16

class Mode_Test;
class Mode_Breath;

struct Config
{
  int saved_flag;

  int current_mode;

  struct Mode
  {
    bool  flag;         // 标记，由各模式自行决定其意义
    float brightest;    // 最亮 0～1
    float darkest;      // 最暗 0～1
    float speed;        // 速度  0～1800次/分钟
    CRGB  color;        // 颜色
  } mode[MODE_NUM];
};

struct DirtyFlags
{
  bool flag;
  bool brightest;
  bool darkest;
  bool speed;
  bool color;

  operator bool() { return flag || brightest || darkest || speed || color; }
};

class CConfigManager
{
  public:
    Config config;
    bool modeChanged;

  public:
    CConfigManager()
    {
      modeChanged = false;
    }

  public:
    void ReadFromEEPROM()
    {
      // 初始化EPROM
      EEPROM.begin(EstimateConfigSize());

      // 判定是不是新设备
      if (EEPROM.read(0) == 0)
      {
        // 新设备，则先恢复出厂设置
        ResetToFactorySettings();
      }

      // 读取配置
      EEPROM.get(0, config);
    }

    void Save()
    {
      // 写入配置
      config.saved_flag += 1;
      EEPROM.put(0, config);

      EEPROM.commit();
    }

  public:
    void PrintConfig()
    {
      Serial.println("================================================================================");
      Serial.println("Config:");

      // 打印EEPROM占用情况
      PrintConfigEEPROMSize();

      // 打印配置值
      PrintConfigValues();

      // 打印格子数据
      PrintGridValues();

      Serial.println("================================================================================");
    }

    void PrintConfigEEPROMSize()
    {
      Serial.print("Size of config : ");
      Serial.println(sizeof(config));
    }

    void PrintConfigValues()
    {
      Serial.println("--------------------------------------------------------------------------------");
      Serial.print("Config.saved_flag = "); Serial.println(config.saved_flag);
      Serial.print("Config.current_mode = "); Serial.println(config.current_mode);
      Serial.println("Mod \tflg \tbrt \tdrk \tspd \tclr");
      Serial.println("--------------------------------------------------------------------------------");
      for (int i = 0; i < MODE_NUM; i++)
      {
        Serial.print(i); Serial.print("\t");
        Serial.print(config.mode[i].flag); Serial.print("\t");
        Serial.print(config.mode[i].brightest); Serial.print(" \t");
        Serial.print(config.mode[i].darkest); Serial.print(" \t");
        Serial.print(config.mode[i].speed); Serial.print(" \t");
        Serial.println(config.mode[i].color, HEX);
      }
      Serial.println("--------------------------------------------------------------------------------");
    }

    void PrintGridValues()
    {
      Serial.println("Grid data:");
      if (Grids.Count() > 0)
      {
        Serial.println("--------------------------------------------------------------------------------");
        Serial.print("r\\c");
        for (int c = 0; c < Grids.GetColNum(); c ++)
        {
          Serial.print("\t"); Serial.print(c);
        }
        Serial.println();

        Serial.println("--------------------------------------------------------------------------------");
        for (int r = 0; r < Grids.GetRowNum(); r++)
        {
          Serial.print(r);
          int x = r * Grids.GetColNum();
          for (int c = 0; c < Grids.GetColNum(); c++)
          {
            Serial.print("\t");
            if (Grids.IsLEDGrid(r, c))
              Serial.print(Grids.GetIndex(r, c));
            else
              Serial.print("--");
          }
          Serial.println();
        }
      }
      else
      {
        Serial.println("NONE");
      }
    }

  public:
    void ResetToFactorySettings()
    {
      Serial.println("This is a NEW DEVICE, Resetting config to DEFAULT...");
      memset(&config, 0, sizeof(config));

      // 配置默认值
      config.saved_flag = 0;
      config.current_mode = 0;
      for (int i = 0; i < MODE_NUM; i++)
      {
        config.mode[i].flag = false;
        config.mode[i].brightest = 0.7;
        config.mode[i].darkest = 0.1;
        config.mode[i].speed = 15;
        config.mode[i].color = CRGB::White;
      }
      
      // 不保存，由命令执行者自行保存
      //Save();
    }

  private:
    size_t EstimateConfigSize()
    {
      return sizeof(Config);
    }
};

static CConfigManager ConfigManager;

#endif
