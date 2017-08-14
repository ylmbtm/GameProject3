using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActClone : ActItem
    {
        public bool CloneObj { get; protected set; }

        public ActClone()
        {
            EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            this.RunClone();
            return true;
        }

        protected void RunClone()
        {
            if (!CloneObj)
            {
                ActClone item = Clone<ActClone>(this);
                item.CloneObj = true;
                item.Skill = Skill;
                item.Clear();
                GTWorld.Instance.Act.Run(item);
                Exit();
            }
        }

    }
}