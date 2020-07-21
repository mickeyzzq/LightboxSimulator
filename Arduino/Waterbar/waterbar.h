#ifndef _WATERBAR_H_
#define _WATERBAR_H_

//////////////////////////////////////////////////////////////////////
// 头文件
#include <EEPROM.h>
#include <FastLED.h>

//////////////////////////////////////////////////////////////////////
// 公用函数声明
// 指令的消息头和尾
#define MSG_HEAD        0x5F    // 二进制为：0101 1111  字符为： _
#define MSG_TERMINATOR  0x5E    // 二进制为：0101 1110  字符为： ^

//////////////////////////////////////////////////////////////////////
// 包含所有的hpp
#include "Version.hpp"
#include "Grids.hpp"
#include "Config.hpp"
#include "Mode.hpp"
#include "Mode_Test.hpp"
#include "Mode_Breath.hpp"
#include "Mode_Mosaic.hpp"
#include "Mode_Audio_Vis.hpp"
#include "Mode_Audio_Scroll.hpp"
#include "CommandProcess.hpp"
//#include "TCPServer.hpp"
#include "UDPServer.hpp"
#include "SerialCommand.hpp"
#endif
