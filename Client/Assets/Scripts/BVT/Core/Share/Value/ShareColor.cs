using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    [Serializable]
    public class ShareColor : ShareValue
    {
        public Color V
        {
            get
            {
                FColor ff = (FColor)data;
                return ff == null ? Color.black : ff.V;
            }
        }

        public override object GetValue()
        {
            return V;
        }

        public override void   SetKey(string key)
        {
            this.SetKey<FColor>(key);
        }
    }
}