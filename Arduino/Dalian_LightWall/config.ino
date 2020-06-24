#include "lightwall.h"

size_t GetConfigNeedBytes()
{
  return 1 + sizeof(g_config);
}

size_t GetMatrixNeedBytes()
{
  if (g_config.led_grid_config.row_num > 0 && g_config.led_grid_config.col_num > 0)
    return g_config.led_grid_config.row_num * g_config.led_grid_config.col_num;
  else
    return DEFAUTL_ROW_NUM * DEFAUTL_COL_NUM;
}
