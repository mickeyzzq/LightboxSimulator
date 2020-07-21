using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System.Net.Sockets;
using System.Text;

public class UDPSender : MonoBehaviour
{
    public UdpClient client;
    public IPAddress serverIp;
    public string hostIp = "192.168.4.1";
    public int hostPort = 9999;
    public IPEndPoint hostEndPoint;

    public int modeIndex = 0;

    public SpeedSlider speed;
    public BrightnessSlider brightness;
    public ColorSlider color;

    void Start()
    {
        serverIp = IPAddress.Parse(hostIp);
        hostEndPoint = new IPEndPoint(serverIp, hostPort);

        client = new UdpClient();
        //client.Connect(hostEndPoint);
        client.Client.Blocking = true;
    }

    public void OnModeSelectChange(int newIndex)
    {
        modeIndex = newIndex;
    }

    public void OnBtnSwitchClicked()
    {
        //Debug.Log(modeIndex);
        //Debug.Log(speed.speedValue);
        //Debug.Log(color.Hex);
        // 组消息
        string msg = string.Format("_{{\"mode\":{0:D}, \"speed\":{1:F}, \"brightest\":{2:F}, \"darkest\":{3:F}, \"color\":\"{4}\"}}$", modeIndex, speed.speedValue, brightness.HighBrightness, brightness.LowBrightness, color.Hex);
        // 发送
        byte[] sendBytes = Encoding.ASCII.GetBytes(msg);
        try
        {
            Debug.Log(msg);
            Debug.Log(sendBytes.Length);
            int num = client.Send(sendBytes, sendBytes.Length, hostEndPoint);
            Debug.Log(num);
        }
        finally
        {
            //byte[] recMsg = client.Receive(ref hostEndPoint);
            //Debug.Log(recMsg);
        }
        
    }

    private bool CheckNetwork()
    {
        return true;
    }
}
