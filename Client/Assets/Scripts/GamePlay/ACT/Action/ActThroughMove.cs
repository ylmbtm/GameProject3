using UnityEngine;
using System.Collections;
using DG.Tweening;

namespace ACT
{
    public class ActThroughMove : ActItem
    {
        [SerializeField]
        public Vector3 EndValueOffset;

        public ActThroughMove()
        {
            EventType = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            if (Skill.Target != null)
            {
                Vector3 endValue = Skill.Target.Pos + EndValueOffset;
                Skill.Caster.CacheTransform.DOMove(endValue, Duration).OnComplete(OnMoveCompleted);
                return true;
            }
            return false;
        }

        protected void OnMoveCompleted()
        {
            Skill.Caster.FaceTarget(0);
        }
    }
}

