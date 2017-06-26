using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "动作节点", Label = "BTLog")]
    public class BTLog : BTTask
    {
        public enum ELogLevel
        {
            TYPE_NORMAL,
            TYPE_WARNING,
            TYPE_ERROR
        }

        [NodeVariable]
        public string    Text  = string.Empty;
        [NodeVariable]
        public ELogLevel Level = ELogLevel.TYPE_NORMAL;

        public override ENST OnExecute()
        {
            switch(Level)
            {
                case ELogLevel.TYPE_NORMAL:
                    Debug.Log(Text);
                    break;
                case ELogLevel.TYPE_WARNING:
                    Debug.LogWarning(Text);
                    break;
                case ELogLevel.TYPE_ERROR:
                    Debug.LogError(Text);
                    break;
            }
            return ENST.SUCCESS;
        }

#if UNITY_EDITOR
        public override void DrawNodeWindowContents()
        {
            if(!string.IsNullOrEmpty(Text))
            {
                Color tempColor = GUI.color;
                switch (Level)
                {
                    case ELogLevel.TYPE_NORMAL:
                        GUI.color = Color.white;
                        break;
                    case ELogLevel.TYPE_WARNING:
                        GUI.color = Color.yellow;
                        break;
                    case ELogLevel.TYPE_ERROR:
                        GUI.color = Color.red;
                        break;
                }
                GUILayout.Label(Text);
                GUI.color = tempColor;
            }
            else
            {
                base.DrawNodeWindowContents();
            }
        }
#endif
    }
}
