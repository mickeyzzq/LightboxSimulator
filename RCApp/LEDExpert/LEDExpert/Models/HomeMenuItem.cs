using System;
using System.Collections.Generic;
using System.Text;

namespace LEDExpert.Models
{
    public enum MenuItemType
    {
        Fantasy,        // 幻彩效果（无输入）
        Vocals,         // 人声效果（麦克风）
        Music,          // 音乐效果（Aux或麦克风，Aux优先）
        About
    }
    public class HomeMenuItem
    {
        public MenuItemType Id { get; set; }

        public string Title { get; set; }
    }
}
