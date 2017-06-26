using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace LVL
{
    public class LevelUtil
    {
        public static void   GetAllComponents<T>(Transform trans, List<T> pList) where T : Component
        {
            if (trans == null) return;
            for (int i = 0; i < trans.childCount; i++)
            {
                T t = trans.GetChild(i).GetComponent<T>();
                if (t != null)
                {
                    pList.Add(t);
                }
            }
        }

        public static string GetConfigPath(int mapID)
        {
            return string.Format("Text/Map/{0}", mapID);
        }
    }
}

