using UnityEngine;
using System.Collections;
using BVT;
using System;

namespace BVT
{
    [Serializable]
    public class ShareBool : ShareValue
    {

        public bool V
        {
            get
            {
                FBool ff = (FBool)data;
                return ff == null ? false : ff.V;
            }
        }

        public override object GetValue()
        {
            return V;
        }

        public override void   SetKey(string key)
        {
            this.SetKey<FBool>(key);
        }
    }
}