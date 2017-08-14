using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActRandom : ActItem
    {
        public int RandomIndex
        {
            get; private set;
        }

        public ActRandom()
        {
            this.EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            this.RandomIndex = UnityEngine.Random.Range(0, Children.Count);
            return true;
        }

        protected override void ExecuteChildren()
        {
            if (Children.Count > 0)
            {
                ActItem item = Children[RandomIndex];
                item.Loop();
                if (item.Status == EActStatus.SUCCESS)
                {
                    Exit();
                }
            }
            else
            {
                Exit();
            }
        }
    }
}