using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActSequence : ActItem
    {
        public int ExecuteIndex
        {
            get; private set;
        }

        public ActSequence()
        {
            EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            this.ExecuteIndex = 0;
            return true;
        }

        protected override void ExecuteChildren()
        {
            if (Children.Count > 0)
            {
                if (ExecuteIndex >= Children.Count)
                {
                    Exit();
                }
                else
                {
                    ActItem item = Children[ExecuteIndex];
                    if (item.Status == EActStatus.SUCCESS)
                    {
                        ExecuteIndex++;
                    }
                }
            }
            else
            {
                Exit();
            }
        }

        public override void Clear()
        {
            base.Clear();
            this.ExecuteIndex = 0;
        }
    }
}
