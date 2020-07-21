#ifndef _MODE_AUDIO_SCROLL_HPP_
#define _MODE_AUDIO_SCROLL_HPP_

#include "Mode.hpp"

#define AUDIO_PIN   35

class Mode_AudioScroll : public Mode
{
  public:
    typedef Mode super;
    Mode_AudioScroll(int midx): Mode(midx)
    {
      _pre_react = 0;
      _react = 0;
      _k = 255;
      _wheel_speed = 1;
      _decay_check = 0;
      _decay = 0;
    }

    static void DefaultConfig(Config::Mode& modecfg)
    {
      modecfg.flag = false;
      modecfg.brightest = 0.65;
      modecfg.darkest = 0;
      modecfg.speed = 3;
      modecfg.color = CRGB::White;
    }

  public:
    virtual void Initialize()
    {
      super::Initialize();
      _wheel_speed = ConfigManager.config.mode[_modeIndex].speed;
      _brightness_max = min((uint8_t)(BRIGHT_RANGE_MAX * ConfigManager.config.mode[_modeIndex].brightest), BRIGHT_RANGE_MAX);
      Grids.SetBrightness(_brightness_max);
      Grids.LightOff();
      Grids.Show();

      _pre_react = 0;
      _react = 0;
      _k = 255;
      _decay_check = 0;
      _decay = 0;

      Serial.println("Audio Vis mode initialized.");
      Serial.print("_wheel_speed = "); Serial.println(_wheel_speed);
      Serial.print("_brightness_max = "); Serial.println(_brightness_max);
    }

    virtual void OnConfigChanged(DirtyFlags& dirtyFlags)
    {
      if (dirtyFlags.speed)
      {
        _wheel_speed = ConfigManager.config.mode[_modeIndex].speed;
      }

      if (dirtyFlags.brightest)
      {
        _brightness_max = max((uint8_t)(BRIGHT_RANGE_MAX * ConfigManager.config.mode[_modeIndex].brightest), BRIGHT_RANGE_MAX);
        Grids.SetBrightness(_brightness_max);
      }
    }

    CRGB Scroll(int pos)
    {
      CRGB color (0, 0, 0);
      if (pos < 85) {
        color.g = 0;
        color.r = ((float)pos / 85.0f) * 255.0f;
        color.b = 255 - color.r;
      } else if (pos < 170) {
        color.g = ((float)(pos - 85) / 85.0f) * 255.0f;
        color.r = 255 - color.g;
        color.b = 0;
      } else if (pos < 256) {
        color.b = ((float)(pos - 170) / 85.0f) * 255.0f;
        color.g = 255 - color.b;
        color.r = 1;
      }
      return color;
    }

    void rainbow()
    {
      for (int i = Grids.GetRowNum() - 1; i >= 0; i--)
      {
          Grids.SetRowColor(i, Scroll((i * 256 / 12 + _k) % 256));
      }
    }

    virtual bool Process()
    {
      int audio_input = analogRead(AUDIO_PIN);
      //Serial.println(audio_input);
      if (audio_input > 1024)
      {
        _k += _wheel_speed;
        if (_k > 255) _k = 1;
        //Grids.SetColor(Scroll(_k));
        rainbow();
      }

      
//      _k = _k - _wheel_speed; // SPEED OF COLOR WHEEL
//      if (_k < 0) // RESET COLOR WHEEL
//        _k = 255;

      return true;
    }

  private:
    long _pre_react;
    long _react;
    int _k;
    int _wheel_speed;
    int _decay_check;
    int _decay;
    uint8_t _brightness_max;
};

#endif
