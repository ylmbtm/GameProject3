using UnityEngine;
using System.Collections;
using BVT;
using System;

namespace BVT
{
    [Serializable]
    public class ShareVector3 : ShareValue
    {
        public Vector3 V
        {
            get
            {
                FVector3 ff = (FVector3)data;
                return ff == null ? Vector3.zero : ff.V;
            }
        }

        public override object GetValue()
        {
            return V;
        }

        public override void   SetKey(string key)
        {
            this.SetKey<FVector3>(key);
        }
    }
}
