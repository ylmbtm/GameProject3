using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    [Serializable]
    public class ShareTransform : ShareValue
    {
        public Transform V
        {
            get
            {
                FTransform ff = (FTransform)data;
                return ff == null ? null : ff.V;
            }
        }

        public override object GetValue()
        {
            return V;
        }

        public override void   SetKey(string key)
        {
            this.SetKey<FTransform>(key);
        }

        public override string ToEncode()
        {
            FTransform ff = (FTransform)data;
            return string.Format("'<color=#FFA400><b>{0}</b></color>'", ff == null ? "null" : ff.name);
        }
    }
}
