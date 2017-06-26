using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BVT
{
    public class BTEventHandler
    {
        public delegate void EventCallback(params object[] args);
        static Dictionary<BTEventID, List<EventCallback>> CallBacks = new Dictionary<BTEventID, List<EventCallback>>();

        static public void AddHandler(BTEventID id, EventCallback callback)
        {
            if (callback == null) return;
            List<EventCallback> pList = null;
            CallBacks.TryGetValue(id, out pList);
            if (pList == null)
            {
                pList = new List<EventCallback>();
                pList.Add(callback);
                CallBacks.Add(id, pList);
            }
            else
            {
                bool isExist = false;
                for (int i = 0; i < pList.Count; i++)
                {
                    if (pList[i] == callback)
                    {
                        isExist = true;
                        break;
                    }
                }
                if (!isExist) pList.Add(callback);
            }
        }

        static public void DelHandler(BTEventID id, EventCallback callback)
        {
            List<EventCallback> pList = null;
            CallBacks.TryGetValue(id, out pList);
            if (pList == null)
            {
                return;
            }
            for (int i = 0; i < pList.Count; i++)
            {
                if (pList[i] == callback)
                {
                    pList.RemoveAt(i);
                }
            }
            if (pList.Count == 0)
            {
                CallBacks.Remove(id);
            }
        }

        static public void FireEvent(BTEventID id, params object[] args)
        {
            List<EventCallback> pList = null;
            CallBacks.TryGetValue(id, out pList);
            if (pList == null)
            {
                return;
            }
            for (int i = 0; i < pList.Count; i++)
            {
                EventCallback callback = pList[i];
                if (callback != null)
                {
                    callback.Invoke(args);
                }
            }
        }
    }
}