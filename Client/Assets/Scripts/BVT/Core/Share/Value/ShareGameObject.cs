using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    [Serializable]
    public class ShareGameObject : ShareValue
    {
        public GameObject V
        {
            get
            {
                FGameObject ff = (FGameObject)data;
                return ff == null ? null : ff.V;
            }
        }

        public override object GetValue()
        {
            return V;
        }

        public override void SetKey(string key)
        {
            this.SetKey<FGameObject>(key);
        }
    }
}
