using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActObj : ActItem
    {
        [SerializeField]
        public int         ID     = 0;
        [SerializeField]
        public Vector3     Offset = Vector3.zero;
        [SerializeField]
        public Vector3     Euler  = Vector3.zero;
        [SerializeField]
        public Vector3     Scale  = Vector3.one;
        [SerializeField]
        public bool        World  = false;
        [SerializeField]
        public bool        Retain = true;

        public EffectData  Unit
        {
            get; protected set;
        }
    }
}

