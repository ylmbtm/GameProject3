using UnityEngine;
using System.Collections;
using BVT;
using System;
using System.Collections.Generic;

namespace BVT
{
    [Serializable]
    public class ShareGameObjectList : ShareValue
    {
        public List<GameObject> V
        {
            get
            {
                FGameObjectList ff = (FGameObjectList)data;
                return ff == null ? null : ff.V;
            }
        }

        public override object GetValue()
        {
            return V;
        }

        public override void   SetKey(string key)
        {
            this.SetKey<FGameObjectList>(key);
        }
    }
}