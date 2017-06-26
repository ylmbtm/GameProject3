using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace BVT
{
    public interface IBlackboard
    {
        bool AddData(string key, object value);
        void DelData(string key);
        void SetData(string key, object value);
        TVal GetData<TVal>(string key) where TVal : FValue;
        bool ContainsKey(string key); 
    }
}

