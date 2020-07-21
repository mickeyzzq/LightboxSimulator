#ifndef _TCPSERVER_HPP_
#define _TCPSERVER_HPP_

#include <WiFi.h>
const char* ssid = "LED控制器";
const char* password =  "led12345";

//IPAddress local_ip(192, 168, 1, 1);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 255, 0);

WiFiServer wifiServer(8000);

class CTCPServer
{
  public:
    void Begin()
    {
      Serial.print("Starting Wifi, SSID|password=");
      Serial.print(ssid); Serial.print("|"); Serial.print(password); Serial.println(".");
      //WiFi.begin(ssid, password);

      WiFi.softAP(ssid, password);
      IPAddress IP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(IP);

      //      wifiServer.on("/", handle_OnConnect);
      //      wifiServer.on("/led1on", handle_led1on);
      //      wifiServer.on("/led1off", handle_led1off);
      //      wifiServer.on("/led2on", handle_led2on);
      //      wifiServer.on("/led2off", handle_led2off);
      //      wifiServer.onNotFound(handle_NotFound);
      wifiServer.begin();
    }

    void Process()
    {
      WiFiClient client = wifiServer.available();

      if (client)
      {
        Serial.println("New Client.");
        //client.setTimeout(5000);

        if (client.connected())
        {
          Serial.println("Client connected.");
          if (client.available())
          {
            Serial.println("Client available.");
            int incomingByte = client.read();
            if (incomingByte == MSG_HEAD)
            {
              String msg = client.readStringUntil(MSG_TERMINATOR);
              Serial.print("TCP msg:"); Serial.println(msg);
              if (msg.length() > 1)
              {
                // 直接处理消息
                ProcessCommandMessage(msg);
              }
              client.print("ok");
            }
            client.stop();
          }
        }
      }
    }

    void PrintInfo()
    {
      Serial.print("SSID="); Serial.println(ssid);
      Serial.print("password="); Serial.println(password);

      IPAddress IP = WiFi.softAPIP();
      Serial.print("AP IP address: "); Serial.println(IP);
    }
};

static CTCPServer TCPServer;

#endif
