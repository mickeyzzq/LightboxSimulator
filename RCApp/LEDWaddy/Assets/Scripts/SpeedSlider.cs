using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SpeedSlider : MonoBehaviour
{
    public Slider mainSlider;
    public InputField numberField;

    public float speedValue { get { return mainSlider.value; } }
    void Start()
    {
        OnSliderValueChanged();
    }

    public void OnSliderValueChanged()
    {
        numberField.text = string.Format("{0:F2}", mainSlider.value);
    }
}
