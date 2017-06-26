using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    [Serializable]
    public class ShareComponent : ShareValue
    {
        public Component V
        {
            get
            {
                FComponent ff = (FComponent)data;
                return ff == null ? null : ff.V;
            }
        }

        public override object GetValue()
        {
            return V;
        }

        public override void   SetKey(string key)
        {
            this.SetKey<FComponent>(key);
        }
    }
}