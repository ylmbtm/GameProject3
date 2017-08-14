using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActCanMove : ActItem
    {
        public ActCanMove()
        {
            EventType = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            this.Skill.CanMove = true;
            return base.Trigger();
        }

        protected override void End()
        {
            this.Skill.CanMove = false;
            this.Skill.Caster.Move.Stop();
            base.End();
        }

        public override void Clear()
        {
            this.Skill.CanMove = false;
            this.Skill.Caster.Move.Stop();
            base.Clear();
        }

    }
}

