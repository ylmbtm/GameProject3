using UnityEngine;
using System.Collections;
using System;

namespace MAP
{
    public class FTGroupEvent : FTGroup<FTEvent>
    {
        public FTEvent AddEvent(Type type)
        {
            FTEvent t = (FTEvent)new GameObject(type.Name).AddComponent(type);
            t.transform.parent = transform;
            t.ID = GetUseID();
            m_Elements.Add(t);
            return t;
        }

        public FTEvent DelEvent(int id)
        {
            for (int i = 0; i < m_Elements.Count; i++)
            {
                FTEvent e = m_Elements[i];
                if (e != null && e.ID == id)
                {
                    GameObject.DestroyImmediate(e.gameObject);
                    m_Elements.RemoveAt(i);
                    break;
                }
            }
            return null;
        }

        public override void OnDrawInspector()
        {

        }
    }
}