#ifndef _UDPSERVER_HPP_
#define _UDPSERVER_HPP_

#include <WiFi.h>
const char* ssid = "LED控制器";
const char* password =  "led12345";

//IPAddress local_ip(192, 168, 1, 1);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 255, 0);
WiFiUDP Udp;
unsigned int localPort = 9999;

class CUDPServer
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

      Udp.begin(localPort);
    }

    void Process()
    {
      int packetSize = Udp.parsePacket();

      if (packetSize)
      {
        char packetBuffer[255];
        int len = Udp.read(packetBuffer, 255);
        if (len > 0) packetBuffer[len - 1] = 0;

        Serial.print("Recibido(IP/Size/Data): ");
        Serial.print(Udp.remoteIP()); Serial.print(" / ");
        Serial.print(packetSize); Serial.print(" / ");
        Serial.println(packetBuffer);

        if (packetBuffer[0] == MSG_HEAD)
        {
          packetBuffer[0] = ' ';
          if (packetBuffer[packetSize - 1] == MSG_TERMINATOR)
          {
            packetBuffer[packetSize - 1] = ' ';
          }
          String msg(packetBuffer);
          ProcessCommandMessage(msg);
        }

        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.printf(packetBuffer);
        Udp.printf("\r\n");
        Udp.endPacket();
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

static CUDPServer UDPServer;

#endif
