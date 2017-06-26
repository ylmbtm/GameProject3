using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActProbably : ActItem
    {
        [SerializeField]
        public float Probably = 0.5f;

        public ActProbably()
        {
            this.EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            return GTTools.IsTrigger(Probably);
        }
    }
}

