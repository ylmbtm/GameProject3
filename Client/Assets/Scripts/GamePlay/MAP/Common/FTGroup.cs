using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

namespace MAP
{
    public class FTGroup<T> : FTContainer where T : FTComponent
    {
        protected      List<T> m_Elements      = new List<T>();

        public         List<T> GetElements()
        {
            return m_Elements;
        }

        public          T      GetElement(int id)
        {
            List<T> list = GetElements();
            for (int i = 0; i < list.Count; i++)
            {
                if (list[i].ID == id)
                {
                    return list[i];
                }
            }
            return null;
        }

        public          T      AddElement() 
        {
            T t = new GameObject(typeof(T).Name).AddComponent<T>();
            t.transform.parent = transform;
            t.ID = GetUseID();
            m_Elements.Add(t);
            return t;
        }

        public          void   DelElement(T item)
        {
            if (item != null)
            {
                m_Elements.Remove(item);
                GameObject.DestroyImmediate(item.gameObject);
            }
        }

        public          Int32  GetUseID()
        {
            HashSet<int> keys = new HashSet<int>();
            List<T>   list = GetElements();
            for (int i = 0; i < list.Count; i++)
            {
                if (keys.Contains(list[i].ID) == false)
                {
                    keys.Add(list[i].ID);
                }
            }
            int index = 1;
            while (keys.Contains(index))
            {
                index++;
            }
            return index;
        }

        public override void   OnDrawGizmos()
        {
            List<T> list = GetElements();
            for (int i = 0; i < list.Count; i++)
            {
                if(list[i] == null)
                {
                    list.RemoveAt(i);
                }
                else
                {
                    list[i].name = string.Format("{0} {1}", list[i].GetType().Name, list[i].ID);
                }
            }
        }

        public override void   OnDrawInspector()
        {
#if UNITY_EDITOR
            GUILayout.Space(10);

            GUILayout.BeginHorizontal();
            GUI.color = Color.green;
            if (GUILayout.Button("添加元素", FTGUIStyles.FTButtonStyle, GUILayout.Height(40)))
            {
                this.AddElement();
            }
            GUILayout.Space(10);
            GUI.color = Color.green;
            if (GUILayout.Button("快捷编辑", FTGUIStyles.FTButtonStyle, GUILayout.Height(40)))
            {
                UnityEditor.EditorWindow.GetWindow<FTComponentWindow>().SetGroup(this);
                UnityEditor.EditorWindow.GetWindow<FTComponentWindow>().Show();
            }

            GUILayout.EndHorizontal();

            GUI.color = Color.red;
            GUILayout.Space(10);
            if (GUILayout.Button("所有目标贴近地面", FTGUIStyles.FTButtonStyle, GUILayout.Height(40)))
            {
                List<T> list = GetElements();
                for (int i = 0; i < list.Count; i++)
                {
                    list[i].MoveToGround();
                }
            }
            GUILayout.Space(10);
#endif
        }

        public override void   AddElement(Vector3 pos, Vector3 eulerAngles)
        {
            T t = AddElement();
            t.transform.position = pos;
            t.transform.eulerAngles = eulerAngles;
        }
    }
}