#ifndef _MODE_MOSAIC_HPP_
#define _MODE_MOSAIC_HPP_

#include "Mode.hpp"

const uint8_t kMatrixWidth = 23;
const uint8_t kMatrixHeight = 12;
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)

class Mode_Mosaic : public Mode
{
  public:
    typedef Mode super;
    Mode_Mosaic(int midx): Mode(midx)
    {
      _speed = 2;
      _scale = 311;
    }

    static void DefaultConfig(Config::Mode& modecfg)
    {
      modecfg.flag = false;
      modecfg.brightest = 0.376;
      modecfg.darkest = 0.375;
      modecfg.speed = 2;
      modecfg.color = CRGB::White;
    }

  public:
    virtual void Initialize()
    {
      super::Initialize();
      _speed = ConfigManager.config.mode[_modeIndex].speed;
      _brightness_max = min((uint8_t)(BRIGHT_RANGE_MAX * ConfigManager.config.mode[_modeIndex].brightest), BRIGHT_RANGE_MAX);
      Grids.SetBrightness(_brightness_max);

      _col = random16();
      _row = random16();
      _deltaTime = random16();


      Serial.println("Mosaic mode initialized.");
      Serial.print("_speed = "); Serial.println(_speed);
      Serial.print("_brightness_max = "); Serial.println(_brightness_max);
    }

    virtual void OnConfigChanged(DirtyFlags& dirtyFlags)
    {
      if (dirtyFlags.speed)
      {
        _speed = ConfigManager.config.mode[_modeIndex].speed;
      }

      if (dirtyFlags.brightest)
      {
        _brightness_max = max((uint8_t)(BRIGHT_RANGE_MAX * ConfigManager.config.mode[_modeIndex].brightest), BRIGHT_RANGE_MAX);
        Grids.SetBrightness(_brightness_max);
      }
    }

    void fillnoise8()
    {
      for (int i = 0; i < MAX_DIMENSION; i++)
      {
        int ioffset = _scale * i;
        for (int j = 0; j < MAX_DIMENSION; j++)
        {
          int joffset = _scale * j;
          noise[i][j] = inoise8(_col + ioffset, _row + joffset, _deltaTime);
        }
      }
      _deltaTime += _speed;
    }

    virtual bool Process()
    {
      static uint8_t ihue = 0;
      fillnoise8();
      for (int i = 0; i < kMatrixWidth; i++)
      {
        for (int j = 0; j < kMatrixHeight; j++)
        {
          // We use the value at the (i,j) coordinate in the noise
          // array for our brightness, and the flipped value from (j,i)
          // for our pixel's hue.
          Grids.SetColor(j, i, CHSV(noise[j][i], 255, noise[i][j]));
          //leds[XY(i, j)] = CHSV(noise[j][i], 255, noise[i][j]);

          // You can also explore other ways to constrain the hue used, like below
          // leds[XY(i,j)] = CHSV(ihue + (noise[j][i]>>2),255,noise[i][j]);
        }
      }
      ihue += 1;
      return true;
    }

  private:
    uint16_t _col;
    uint16_t _row;
    uint16_t _deltaTime;
    uint16_t _speed;
    uint16_t _scale;
    uint8_t _brightness_max;
    uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
};

#endif
