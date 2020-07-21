#ifndef _MODE_TEST_HPP_
#define _MODE_TEST_HPP_

#include "Mode.hpp"

// Mode Index : 0
// 模式：灯光测试模式
//      按格子，顺序点亮每个灯，每个循环换一种颜色，白红绿蓝，每格子点亮0.2秒，每秒5格，每分钟300格，60格需要12秒

CRGB ColorLoop[4] = { 0xFFFFFF, 0xFF0000, 0x00FF00, 0x0000FF };

class Mode_Test: public Mode
{
  public:
    typedef Mode super;
    Mode_Test(int midx): Mode(midx)
    {
      _interval_time = 200;
      _current_color = CRGB::White;
      _row = 0;
      _col = 0;
    }

    static void DefaultConfig(Config::Mode& modecfg)
    {
      modecfg.flag = false;
      modecfg.brightest = 0.7;
      modecfg.darkest = 0.1;
      modecfg.speed = 60;
      modecfg.color = CRGB::White;
    }

  public:
    virtual void Initialize()
    {
      super::Initialize();

      _last_proc_time = 0;
      _interval_time = 200;
      if (ConfigManager.config.mode[_modeIndex].speed > 0)
      {
        _interval_time = (1000 * 60) / ConfigManager.config.mode[_modeIndex].speed;
      }
      _row = 0;
      _col = 0;
      _current_color = CRGB::White;
      Grids.LightOff();  // 先全部关闭
    }

    virtual void OnConfigChanged(DirtyFlags& dirtyFlags)
    {
      if (dirtyFlags.speed)
      {
        _interval_time = (1000 * 60) / ConfigManager.config.mode[_modeIndex].speed;
      }
    }

    virtual bool Process()
    {
      unsigned long currMillis = millis();
      if ((currMillis - _last_proc_time) > _interval_time)
      {
        //Serial.print("Turn off:"); Serial.print(_row);  Serial.print(", "); Serial.println(_col);
        Grids.LightOff(_row, _col);
        _col += 1;
        if (FindNextLedGrid(_row, _col))
        {
          //Serial.print("Turn on :"); Serial.print(_row);  Serial.print(", "); Serial.println(_col);
          Grids.SetColor(_row, _col, _current_color);
        }
        else
        {
          _row = 0;
          _col = 0;
          _current_color = NextColor();
          Serial.println("====> Not found, restart test...");
        }
        _last_proc_time = currMillis;
        //Serial.println();
        return true;
      }
      else
      {
        return false;
      }
    }

  private:
    CRGB NextColor()
    {
      static int idx = 0;
      idx++;
      if (idx >= 4)
        idx = 0;
      return ColorLoop[idx];
    }
  protected:
    CRGB _current_color;
    unsigned long _last_proc_time;
    unsigned long _interval_time;
    uint8_t _row;
    uint8_t _col;
};
#endif
