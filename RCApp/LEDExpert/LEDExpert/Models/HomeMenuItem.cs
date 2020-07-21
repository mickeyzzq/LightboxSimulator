using System;
using System.Collections.Generic;
using System.Text;

namespace LEDExpert.Models
{
    public enum MenuItemType
    {
        Fantasy,        // 幻彩效果（无输入）
        About
    }
    public class HomeMenuItem
    {
        public MenuItemType Id { get; set; }

        public string Title { get; set; }
    }
}
