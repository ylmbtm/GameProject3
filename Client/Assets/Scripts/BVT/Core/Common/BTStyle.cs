using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BVT
{
    public class BTStyle
    {
        public const string BVT_Bg                        = "BVT_Bg";
        public const string BVT_BgLine                    = "BVT_BgLine";
        public const string BVT_Arrow                     = "BVT_Arrow";
        public const string BVT_Arrow_Top                 = "BVT_Arrow_Top";
        public const string BVT_Arrow_Bottom              = "BVT_Arrow_Bottom";
        public const string BVT_Arrow_Left                = "BVT_Arrow_Left";
        public const string BVT_Arrow_Right               = "BVT_Arrow_Right";

        public const string BVT_Btn_Hover                 = "BVT_Btn_Hover";
        public const string BVT_Btn_Normal                = "BVT_Btn_Normal";
        public const string BVT_Checkmark                 = "BVT_Checkmark";
        public const string BVT_ClockMark                 = "BVT_ClockMark";

        public const string BVT_NodePort                  = "BVT_NodePort";
        public const string BVT_NodePortConnectedHover    = "BVT_NodePortConnectedHover";
        public const string BVT_NodePortHover             = "BVT_NodePortHover";
        public const string BVT_Panel                     = "BVT_Panel";
        public const string BVT_ScaleArrow                = "BVT_ScaleArrow";
        public const string BVT_ScaleArrowHover           = "BVT_ScaleArrowHover";
        public const string BVT_SimpleBox                 = "BVT_SimpleBox";
        public const string BVT_SimpleInnerShadow         = "BVT_SimpleInnerShadow";

        public const string BVT_Window                    = "BVT_Window";
        public const string BVT_WindowHighlight           = "BVT_WindowHighlight";
        public const string BVT_WindowHover               = "BVT_WindowHover";
        public const string BVT_WindowHoverLight          = "BVT_WindowHoverLight";
        public const string BVT_WindowShadow              = "BVT_WindowShadow";
        public const string BVT_XMark                     = "BVT_XMark";

        private static GUIStyle mStyleCenterLabel;

        public static GUIStyle  STYLE_CENTERLABEL
        {
            get
            {
                if(mStyleCenterLabel==null)
                {
                    mStyleCenterLabel = new GUIStyle("label");
                    mStyleCenterLabel.alignment = TextAnchor.UpperCenter;
                    mStyleCenterLabel.richText = true;
                    mStyleCenterLabel.fontSize = 14;
                }
                return mStyleCenterLabel;
            }
        }
         
    }
}
