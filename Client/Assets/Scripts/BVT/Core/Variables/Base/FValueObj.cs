using UnityEngine;
using System.Collections;

namespace BVT
{
    public class FValueObj<T> : FValue
    {
        public T V;

        public override object GetValue()
        {
            return V;
        }

        public override string ToEncode()
        {
            return string.Format("{0}:{1}", K, V);
        }

        public override void   SetValue(object v)
        {
            this.V = (T)v;
        }
    }
}

