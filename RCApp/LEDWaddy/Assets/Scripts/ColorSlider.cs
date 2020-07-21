using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ColorSlider : MonoBehaviour
{
    public Slider R_Slider;
    public Slider G_Slider;
    public Slider B_Slider;
    public InputField R_Field;
    public InputField G_Field;
    public InputField B_Field;
    public Image colorImg;

    private Color myColor = new Color(1,1,1);

    public int R { get { return (int)R_Slider.value; } }
    public int G { get { return (int)G_Slider.value; } }
    public int B { get { return (int)B_Slider.value; } }

    public string Hex { get { return string.Format("{0:X2}{1:X2}{2:X2}", R, G, B); } }
    public Color color { get { return myColor; } }

    void Start()
    {
        R_Field.text = string.Format("{0:D}", R);
        G_Field.text = string.Format("{0:D}", G);
        B_Field.text = string.Format("{0:D}", B);
        RefreshColor();
    }

    public void OnR_SliderValueChanged()
    {
        R_Field.text = string.Format("{0:D}", R);
        RefreshColor();
    }
    public void OnG_SliderValueChanged()
    {
        G_Field.text = string.Format("{0:D}", G);
        RefreshColor();
    }
    public void OnB_SliderValueChanged()
    {
        B_Field.text = string.Format("{0:D}", B);
        RefreshColor();
    }

    public void RefreshColor()
    {
        myColor.r = (float)R / 255;
        myColor.g = (float)G / 255;
        myColor.b = (float)B / 255;
        colorImg.color = myColor;
    }
}
