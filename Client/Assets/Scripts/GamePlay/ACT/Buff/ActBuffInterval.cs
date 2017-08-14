using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActBuffInterval : ActBuffItem
    {
        [SerializeField]
        public float TickInterval  = 1;
        [SerializeField]
        public bool  PlayOnTrigger = true;
    }
}

