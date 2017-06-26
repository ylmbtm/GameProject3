using BVT;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

namespace BVT
{
    public class FValue : MonoBehaviour
    {
        public string         K = string.Empty;

        public string         GetKey()
        {
            return K;
        }

        public string         SetKey(string key)
        {
            this.K = key;
            return K;
        }

        public virtual object GetValue()
        {
            return null;
        }

        public virtual string ToEncode()
        {
            return null;
        }

        public virtual void   SetValue(object v)
        {
            
        }

#if UNITY_EDITOR
        public virtual void   DrawGUI()
        {
            GUILayout.BeginHorizontal();
            if (!Application.isPlaying)
            {
                GUI.color = Color.white;
                GUI.backgroundColor = new Color(0.5f, 0.5f, 0.5f, 1f);
                this.K = UnityEditor.EditorGUILayout.TextField(K,GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
            }
            else
            {
                GUI.backgroundColor = new Color(0.7f, 0.7f, 0.7f);
                GUI.color = new Color(0.8f, 0.8f, 1f);
                GUILayout.Label(K, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
            }
            GUI.color = Color.white;
            GUI.backgroundColor = Color.white;
            DrawField();
            GUILayout.EndHorizontal();
        }

        public         void   DrawKey()
        {

        }

        public virtual void   DrawField()
        {

        }
#endif
    }
}
