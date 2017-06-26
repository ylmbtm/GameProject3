using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    [Serializable]
    public class ShareValue
    {
        [SerializeField]
        public string        key       = string.Empty;
        [SerializeField]
        public FValue        data;
        [SerializeField]
        public NodeBlackboard board;
  
        public virtual object GetValue()
        {
            return null;
        }

        public virtual void   SetValue(object v, bool syncToData = true)
        {
            if (syncToData && data != null)
            {
                data.SetValue(v);
            }
        }

        public virtual void   SetKey(string key)
        {

        }

        public         string GetKey()
        {
            return key;
        }

        public virtual string ToEncode()
        {
            return string.Format("'<color=#FFA400><b>{0}</b></color>'", GetValue());
        }

        public         void   SetBlackBoard(NodeBlackboard board)
        {
            this.board = board;
        }

        public         void   SetKey<T>(string key) where T : FValue
        {
            if (this.key == key)
            {
                return;
            }
            this.key = key;
            T f = board.GetData<T>(key);
            data = (T)f;
            if (data != null)
            {
                data.SetKey(key);
                this.SetValue(data.GetValue(), false);
            }
        }

#if UNITY_EDITOR
        public         void   DrawGUI(string label)
        {
            GUI.color = Color.white;
            GUILayout.Label(string.Format("<color=#0FE3FF>[ShareObject]</color> {0}", label), BTHelper.BVTSkin.label,GUILayout.MaxWidth(200));
            GUILayout.BeginHorizontal();
            if (!Application.isPlaying)
            {
                GUI.color = Color.white;
                GUI.backgroundColor = new Color(0.5f, 0.5f, 0.5f, 1f);
                string k = UnityEditor.EditorGUILayout.TextField(key,GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
                this.SetKey(k);
            }
            else
            {
                GUI.backgroundColor = new Color(0.7f, 0.7f, 0.7f);
                GUI.color = new Color(0.8f, 0.8f, 1f);
                GUILayout.Label(key,GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
            }
            GUI.color = Color.white;
            GUI.backgroundColor = Color.white;

            if (data == null)
            {
                UnityEditor.EditorGUILayout.ObjectField("", data, typeof(FValue), true, GUILayout.MaxWidth(100), GUILayout.ExpandWidth(true));
            }
            else
            {
                data.DrawField();
                if (data.K != key)
                {
                    this.key = data.K;
                }
                if (data.GetValue() != GetValue())
                {
                    this.SetValue(data.GetValue(), false);
                }
            }
            GUILayout.EndHorizontal();
        }
#endif
    }
}