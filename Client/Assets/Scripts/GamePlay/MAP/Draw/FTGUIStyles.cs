using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTGUIStyles
    {
        private static GUIStyle m_FTButtonStyle;

        public static GUIStyle FTButtonStyle
        {
            get
            {
                if (m_FTButtonStyle == null)
                {
                    m_FTButtonStyle = new GUIStyle("button");
                    m_FTButtonStyle.alignment = TextAnchor.MiddleCenter;
                    m_FTButtonStyle.fontSize = 24;
                }
                return m_FTButtonStyle;
            }
        }
    }
}