using UnityEngine;
using System.Collections;
using BVT;
using System;

namespace BVT
{
    [Serializable]
    public class ShareInt : ShareValue
    {
        public int V
        {
            get
            {
                FInt ff = (FInt)data;
                return ff == null ? 0 : ff.V;
            }
        }

        public override object GetValue()
        {
            FInt ff = (FInt)data;
            return ff == null ? 0 : ff.V;
        }

        public override void   SetKey(string key)
        {
            this.SetKey<FInt>(key);
        }
    }
}
