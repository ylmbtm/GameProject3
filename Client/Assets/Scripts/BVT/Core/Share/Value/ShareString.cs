using UnityEngine;
using System.Collections;
using BVT;
using System;

namespace BVT
{
    [Serializable]
    public class Share : ShareValue
    {
        public String V
        {
            get
            {
                FString ff = (FString)data;
                return ff == null ? string.Empty : ff.V;
            }
        }

        public override object GetValue()
        {
            return V;
        }

        public override void   SetKey(string key)
        {
            this.SetKey<FString>(key);
        }
    }
}
