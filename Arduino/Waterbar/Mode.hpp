#ifndef _MODE_HPP_
#define _MODE_HPP_

class Mode
{
  public:
    Mode(int midx):
      _modeIndex(midx),
      _needInitialize(true)
    {
    }

  public:
    bool NeedInitialize()
    {
      return _needInitialize;
    }
    
    virtual void Initialize()
    {
      _needInitialize = false;
    }

    virtual void OnConfigChanged(DirtyFlags& dirtyFlags)
    {
      
    }

    virtual bool Process()
    {
      return false;
    }

  public:
    virtual void OnConfigChanged()
    {
      _needInitialize = true;
    }

  protected:
    bool FindNextLedGrid(uint8_t& row, uint8_t& col)
    {
      while (row < Grids.GetRowNum())
      {
        while (col < Grids.GetColNum())
        {
          if (Grids.IsLEDGrid(row, col))
          {
            return true;
          }
          col++;
        }
        row++;
        col = 0;
      }

      return false;
    }

  protected:
    int _modeIndex;
    bool _needInitialize;
};
#endif
