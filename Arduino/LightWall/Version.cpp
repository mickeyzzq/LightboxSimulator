#include <Arduino.h>
#include "lightwall.h"

void PrintVersionInfo()
{
  Serial.println(" _       _                           _      ___    ___   ");
  Serial.println("( )  _  ( ) _                       ( )    (  _`\\ (  _`\\ ");
  Serial.println("| | ( ) | |(_) _ __   ___    _ _    | |    | (_(_)| | ) |");
  Serial.println("| | | | | || |( '__)/'___) /'_` )   | |  _ |  _)_ | | | )");
  Serial.println("| (_/ \\_) || || |  ( (___ ( (_| |   | |_( )| (_( )| |_) |");
  Serial.println("`\\___x___/'(_)(_)  `\\____)`\\__,_)   (____/'(____/'(____/'");
  Serial.println();                                                           
  Serial.println();                                                           
  Serial.println(" ___                  _                _    _                ");
  Serial.println("(  _`\\               ( )_             (_ ) (_ )              ");
  Serial.println("| ( (_)   _     ___  | ,_) _ __   _    | |  | |    __   _ __ ");
  Serial.println("| |  _  /'_`\\ /' _ `\\| |  ( '__)/'_`\\  | |  | |  /'__`\\( '__)");
  Serial.println("| (_( )( (_) )| ( ) || |_ | |  ( (_) ) | |  | | (  ___/| |   ");
  Serial.println("(____/'`\\___/'(_) (_)`\\__)(_)  `\\___/'(___)(___)`\\____)(_)   ");
  Serial.println();                                                           
  Serial.println(" Wirca 高性能LED远程控制器  版本 1.0.1.");
  Serial.println(" Wirca LED Controller Version 1.0.1.");
  Serial.println(" 作者：朱志强");
  Serial.println(" Author: mickeyzzq.");
  Serial.println(" Email/邮箱:mickey@vip.qq.com");
  Serial.println(" Wechat/微信： mickeyzzq");
  Serial.println(" @Copyright 2020，版权所有，盗版必究！");
}
