using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    [Serializable]
    public class ShareFloat : ShareValue
    {
        public float V
        {
            get
            {
                FFloat ff = (FFloat)data;
                return ff == null ? 0 : ff.V;
            }
        }

        public override object GetValue()
        {
            return V;
        }

        public override void   SetKey(string key)
        {
            this.SetKey<FFloat>(key);
        }
    }
}