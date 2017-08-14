using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace ACT
{
    public class ActTree : ActItem
    {
        Dictionary<string, object> Dict = new Dictionary<string, object>();

        public ActTree()
        {
            EventType = EActEventType.Instant;
        }

        public object GetData(string key)
        {
            if (string.IsNullOrEmpty(key))
            {
                return null;
            }
            else
            {
                object obj = null;
                Dict.TryGetValue(key, out obj);
                return obj;
            }
        }

        public T      GetData<T>(string key)
        {
            object obj = GetData(key);
            if (obj == null)
            {
                return default(T);
            }
            else
            {
                return (T)obj;
            }
        }

        public object SetData(string key, object v)
        {
            if (string.IsNullOrEmpty(key))
            {
                return null;
            }
            Dict[key] = v;
            return v;
        }
    }
}
