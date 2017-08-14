using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActFaceToTarget : ActItem
    {
        public ActFaceToTarget()
        {
            this.EventType = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            this.Skill.Caster.FaceTarget(Duration);
            return true;
        }
    }
}

