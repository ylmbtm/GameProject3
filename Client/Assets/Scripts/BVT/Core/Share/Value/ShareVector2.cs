using UnityEngine;
using System.Collections;
using BVT;
using System;

namespace BVT
{
    [Serializable]
    public class ShareVector2 : ShareValue
    {
        public Vector2 V
        {
            get
            {
                FVector2 ff = (FVector2)data;
                return ff == null ? Vector2.zero : ff.V;
            }
        }

        public override object GetValue()
        {
            return V;
        }

        public override void   SetKey(string key)
        {
            this.SetKey<FVector2>(key);
        }
    }
}