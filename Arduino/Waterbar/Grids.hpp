#ifndef _GRIDS_HPP_
#define _GRIDS_HPP_

#include "SampleMap.h"

#ifdef RELEASE
#define LED_CHIP_SET          WS2811_400 //UCS1903
#define LED_DATA_PIN          23
#define LED_CHIP_RGBSET       BGR

#define GKEY(__r, __c)            (__r*1000 + __c)
#define GINFO_12(__r, __c, idx)   (__r*1000 + __c), new GridInfo(__r, __c, idx, (idx - 1) * 12, 12)
#else
#define LED_CHIP_SET          WS2811 //UCS1903
#define LED_DATA_PIN          23
#define LED_CHIP_RGBSET       BGR

#define GKEY(__r, __c)            (__r*1000 + __c)
#define GINFO_12(__r, __c, idx)   (__r*1000 + __c), new GridInfo(__r, __c, idx, (idx - 1) * 1, 1)
#endif

class CGrids
{
  public:             // 初始化
    CGrids()
    {
      _myMap = new SimpleMap<long, GridInfo*>([](long & a, long & b) -> int {
        if (a == b) return 0;

        if (a > b) return 1;

        /*if (a < b) */ return -1;
      });

      // 12芯片的格子
      _myMap->put( GINFO_12( 0,  7, 1) );
      _myMap->put( GINFO_12( 0,  9, 2) );
      _myMap->put( GINFO_12( 0, 17, 3) );
      _myMap->put( GINFO_12( 1, 18, 4) );
      _myMap->put( GINFO_12( 1, 16, 5) );
      _myMap->put( GINFO_12( 1, 12, 6) );
      _myMap->put( GINFO_12( 1, 10, 7) );
      _myMap->put( GINFO_12( 1,  5, 8) );
      _myMap->put( GINFO_12( 1,  4, 9) );
      _myMap->put( GINFO_12( 2,  3, 10) );
      _myMap->put( GINFO_12( 2,  4, 11) );
      _myMap->put( GINFO_12( 2,  9, 12) );
      _myMap->put( GINFO_12( 2, 13, 13) );
      _myMap->put( GINFO_12( 2, 15, 14) );
      _myMap->put( GINFO_12( 2, 16, 15) );
      _myMap->put( GINFO_12( 3, 20, 16) );
      _myMap->put( GINFO_12( 3, 17, 17) );
      _myMap->put( GINFO_12( 4, 18, 18) );
      _myMap->put( GINFO_12( 4, 19, 19) );
      _myMap->put( GINFO_12( 5, 21, 20) );
      _myMap->put( GINFO_12( 5, 20, 21) );
      _myMap->put( GINFO_12( 5, 19, 22) );
      _myMap->put( GINFO_12( 6, 18, 23) );
      _myMap->put( GINFO_12( 7, 19, 24) );
      _myMap->put( GINFO_12( 9, 15, 25) );
      _myMap->put( GINFO_12( 8, 14, 26) );
      _myMap->put( GINFO_12( 8, 15, 27) );
      _myMap->put( GINFO_12( 7, 15, 28) );
      _myMap->put( GINFO_12( 6, 16, 29) );
      _myMap->put( GINFO_12( 5, 16, 30) );
      _myMap->put( GINFO_12( 5, 15, 31) );
      _myMap->put( GINFO_12( 4, 15, 32) );
      _myMap->put( GINFO_12( 3, 16, 33) );
      _myMap->put( GINFO_12( 3, 11, 34) );

      // 以下为30灯珠（10芯片）
      _myMap->put( GINFO_12( 3, 10, 35) );
      _myMap->put( GINFO_12( 3,  8, 36) );
      _myMap->put( GINFO_12( 3,  7, 37) );
      _myMap->put( GINFO_12( 3,  1, 38) );
      _myMap->put( GINFO_12( 4,  1, 39) );
      _myMap->put( GINFO_12( 4,  5, 40) );
      _myMap->put( GINFO_12( 4,  6, 41) );
      _myMap->put( GINFO_12( 4,  9, 42) );
      _myMap->put( GINFO_12( 4, 10, 43) );
      _myMap->put( GINFO_12( 5,  9, 44) );
      _myMap->put( GINFO_12( 5,  8, 45) );
      _myMap->put( GINFO_12( 5,  6, 46) );
      _myMap->put( GINFO_12( 5,  5, 47) );
      _myMap->put( GINFO_12( 5,  4, 48) );
      _myMap->put( GINFO_12( 6,  4, 49) );
      _myMap->put( GINFO_12( 6,  5, 50) );
      _myMap->put( GINFO_12( 6,  9, 51) );
      _myMap->put( GINFO_12( 6, 10, 52) );
      _myMap->put( GINFO_12( 7,  8, 53) );
      _myMap->put( GINFO_12( 7,  7, 54) );
      _myMap->put( GINFO_12( 7,  5, 55) );
      _myMap->put( GINFO_12( 8,  9, 56) );
      _myMap->put( GINFO_12( 7, 12, 57) );
      _myMap->put( GINFO_12( 6, 13, 58) );
      _myMap->put( GINFO_12( 5, 13, 59) );
      _myMap->put( GINFO_12( 4, 13, 60) );

      GridInfo* pInfo = _myMap->get( GKEY(4, 13) );
      if (pInfo != NULL)
      {
        _ledCount = pInfo->ic_start + pInfo->ic_num;
      }

      _rowNum = 12;
      _colNum = 23;
    }

  public:                   // 格子属性
    int Count()
    {
      return _myMap->size();
    }

    inline int GetRowNum() {
      return _rowNum;
    }
    
    inline int GetColNum() {
      return _colNum;
    }

    bool IsLEDGrid(uint8_t row, uint8_t col)
    {
      return _myMap->has( GKEY(row, col) );
    }

    int GetIndex(uint8_t row, uint8_t col)
    {
      //return _myMap->getIndex( GKEY(row, col) );
      GridInfo* pInfo = _myMap->get( GKEY(row, col) );
      if (pInfo != NULL)
      {
        return pInfo->wireIndex;
      }
    }

  public:                 // LED 操作
    void InitializeLEDArray()
    {
      Serial.println("Initialize LED driver...");
      Serial.print("  Led Count = "); Serial.println(_ledCount);
      FastLED.addLeds<LED_CHIP_SET, LED_DATA_PIN, LED_CHIP_RGBSET>(_leds, _ledCount);
      SetColor(CRGB::White);
      SetBrightness(255);
      Show();
      delay(10);
    }

    void Show()
    {
      //LEDS.show();
      FastLED.show();
    }

    void SetBrightness(uint8_t brightness)
    {
      FastLED.setBrightness(brightness);
    }

    void SetColor(CRGB color)
    {
      fill_solid(&_leds[0], _ledCount, color);
    }

    void LightOff()
    {
      fill_solid(&_leds[0], _ledCount, CRGB::Black);
      fill_solid(&_leds[0], 100, CRGB::Black);
    }

    void SetColor(uint8_t row, uint8_t col, CRGB color)
    {
      if (IsLEDGrid(row, col))
      {
        GridInfo* pInfo = _myMap->get( GKEY(row, col) );
        if (pInfo != NULL)
        {
          fill_solid( &_leds[pInfo->ic_start], pInfo->ic_num, color);
        }
      }
    }

    void SetRowColor(uint8_t row, CRGB color)
    {
      for (int col = 0; col < GetColNum(); col++)
      {
        if (IsLEDGrid(row, col))
        {
          GridInfo* pInfo = _myMap->get( GKEY(row, col) );
          if (pInfo != NULL)
          {
            fill_solid( &_leds[pInfo->ic_start], pInfo->ic_num, color);
          }
        }
      }
    }

    void LightOff(uint8_t row, uint8_t col)
    {
      if (IsLEDGrid(row, col))
      {
        GridInfo* pInfo = _myMap->get( GKEY(row, col) );
        if (pInfo != NULL)
        {
          if (pInfo->ic_num > 1)
            fill_solid( &_leds[pInfo->ic_start], pInfo->ic_num, CRGB::Black);
          else
            _leds[pInfo->ic_start] = CRGB::Black;
        }
      }
    }

    void LightOffRow(uint8_t row)
    {
      for (int col = 0; col < GetColNum(); col++)
      {
        if (IsLEDGrid(row, col))
        {
          GridInfo* pInfo = _myMap->get( GKEY(row, col) );
          if (pInfo != NULL)
          {
            fill_solid( &_leds[pInfo->ic_start], pInfo->ic_num, CRGB::Black);
          }
        }
      }
    }
  public:
    struct GridInfo
    {
      uint8_t row;
      uint8_t col;
      uint8_t wireIndex;
      int ic_start;
      int ic_num;

      GridInfo()
      {
        row = 0; col = 0; wireIndex = 0; ic_start = -1; ic_num = -1;
      }

      GridInfo(uint8_t r, uint8_t c, uint8_t wi, int s, int n)
      {
        row = r; col = c; wireIndex = wi; ic_start = s; ic_num = n;
      }
    };

  private:
    SimpleMap<long, GridInfo*>* _myMap;
    int _rowNum;
    int _colNum;
    int _ledCount;
    CRGB  _leds[1024];
};

static CGrids Grids;
#endif
