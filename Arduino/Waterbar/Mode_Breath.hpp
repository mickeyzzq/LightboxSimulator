#ifndef _MODE_BREATH_HPP_
#define _MODE_BREATH_HPP_

#include "Mode.hpp"

enum BreathDir { BD_IN, BD_OUT };
const uint8_t BRIGHT_RANGE_MAX  = 255;
const uint8_t BRIGTH_RANGE_MIN  = 1;

class Mode_Breath : public Mode
{
  public:
    typedef Mode super;
    Mode_Breath(int midx): Mode(midx)
    {
      _current_color = CRGB::White;
      breath_dir = BD_IN;
      _half_breath_time = 5000;
      _last_breath_time = 0;
      _brightness_min = 1;
      _brightness_max = 255;
      _current_brightness = 255;
    }

    static void DefaultConfig(Config::Mode& modecfg)
    {
      modecfg.flag = false;
      modecfg.brightest = 0.7;
      modecfg.darkest = 0.1;
      modecfg.speed = 15;
      modecfg.color = CRGB::White;
    }

  public:
    virtual void Initialize()
    {
      super::Initialize();

      if (ConfigManager.config.mode[_modeIndex].speed > 0)
      {
        _half_breath_time = ((1000 * 60) / ConfigManager.config.mode[_modeIndex].speed) / 2;
      }

      _brightness_min = min((uint8_t)(BRIGHT_RANGE_MAX * ConfigManager.config.mode[_modeIndex].darkest), BRIGTH_RANGE_MIN);
      _brightness_max = max((uint8_t)(BRIGHT_RANGE_MAX * ConfigManager.config.mode[_modeIndex].brightest), BRIGHT_RANGE_MAX);

      if (_brightness_min > _brightness_max)
      {
        _brightness_min = _brightness_max;
      }

      _current_brightness = 255;
      _current_color = ConfigManager.config.mode[_modeIndex].color;
      Grids.SetColor(_current_color);
      _last_breath_time = 0;
      breath_dir == BD_IN;
    }

    virtual void OnConfigChanged(DirtyFlags& dirtyFlags)
    {
      if (dirtyFlags.speed)
      {
        _half_breath_time = ((1000 * 60) / ConfigManager.config.mode[_modeIndex].speed) / 2;
      }

      if (dirtyFlags.darkest)
      {
        _brightness_min = min((uint8_t)(BRIGHT_RANGE_MAX * ConfigManager.config.mode[_modeIndex].darkest), BRIGTH_RANGE_MIN);
      }

      if (dirtyFlags.brightest)
      {
        _brightness_max = max((uint8_t)(BRIGHT_RANGE_MAX * ConfigManager.config.mode[_modeIndex].brightest), BRIGHT_RANGE_MAX);
      }

      if (_brightness_min > _brightness_max)
      {
        _brightness_min = _brightness_max;
      }

      if (dirtyFlags.color)
      {
        _current_color = ConfigManager.config.mode[_modeIndex].color;
        Grids.SetColor(_current_color);
      }
    }

    virtual bool Process()
    {
      unsigned long currMillis = millis();
      unsigned long deltaMillis = currMillis - _last_breath_time;

      uint8_t oldBrightness = _current_brightness;
      if (breath_dir == BD_IN)
      {
        _current_brightness = map(deltaMillis, 0, _half_breath_time, _brightness_min, _brightness_max);
      }
      else
      {
        _current_brightness = map(_half_breath_time - deltaMillis, 0, _half_breath_time, _brightness_min, _brightness_max);
      }

      if ( deltaMillis > _half_breath_time )
      {
        // 反向
        breath_dir = breath_dir == BD_IN ? BD_OUT : BD_IN;
        _last_breath_time = currMillis;
      }

      if (oldBrightness != _current_brightness)
      {
        Grids.SetBrightness(_current_brightness);
        return true;
      }
      else
      {
        return super::Process();
      }
    }
  private:
    CRGB _current_color;
    uint8_t _current_brightness;
    BreathDir breath_dir;
    unsigned long _half_breath_time;
    unsigned long _last_breath_time;
    uint8_t _brightness_min;
    uint8_t _brightness_max;
};
#endif
