using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BrightnessSlider : MonoBehaviour
{
    public Slider highSlider;
    public Slider lowSlider;
    public InputField highNumberField;
    public InputField lowNumberField;

    public float HighBrightness { get { return highSlider.value; } }
    public float LowBrightness { get { return lowSlider.value; } }

    void Start()
    {
        OnHighSliderValueChanged();
        OnLowSliderValueChanged();
    }

    public void OnHighSliderValueChanged()
    {
        highNumberField.text = string.Format("{0:F2}", HighBrightness);
        lowSlider.value = Mathf.Min(HighBrightness, lowSlider.value);
    }
    public void OnLowSliderValueChanged()
    {
        lowNumberField.text = string.Format("{0:F2}", LowBrightness);
        highSlider.value = Mathf.Max(LowBrightness, highSlider.value);
    }
}
