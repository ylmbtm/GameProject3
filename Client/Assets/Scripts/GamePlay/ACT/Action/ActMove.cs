using UnityEngine;
using System.Collections;
using DG.Tweening;

namespace ACT
{
    public class ActMove : ActItem
    {
        public enum MoveType
        {
            MOVE_TODIRECTION = 0,
            MOVE_TOTARGET    = 1,
            MOVE_BACK        = 2,
        }

        [SerializeField]
        public MoveType  Type   = MoveType.MOVE_TODIRECTION;
        [SerializeField]
        public float     MaxDis = 10;

        public ActMove()
        {
            this.EventType = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            Vector3 rayStartPoint = Skill.Caster.Pos + new Vector3(0, Skill.Caster.Height / 2, 0);
            switch (Type)
            {
                case MoveType.MOVE_TODIRECTION:
                    {
                        Vector3 endValue = Skill.Caster.Pos + Skill.Caster.Dir * MaxDis;      
                        Skill.Caster.CheckMoveEndPointInFront(ref endValue, MaxDis);
                        Skill.Caster.CacheTransform.DOMove(endValue, Duration);
                    }
                    break;
                case MoveType.MOVE_TOTARGET:
                    {
                        if (Skill.Target == null)
                        {
                            Vector3 endValue = Skill.Caster.Pos + Skill.Caster.Dir * MaxDis;
                            Skill.Caster.CheckMoveEndPointInFront(ref endValue, MaxDis);
                            Skill.Caster.CacheTransform.DOMove(endValue, Duration);
                        }
                        else
                        {
                            Vector3 targetPos = Skill.Target.Pos;
                            Vector3 casterPos = Skill.Caster.Pos;
                            Vector3 dir = targetPos - casterPos;
                            dir.y = 0;
                            dir.Normalize();
                            Vector3 endValue = Skill.Target.Pos - dir * (Skill.Caster.Radius + Skill.Target.Radius);
                            Skill.Caster.CheckMoveEndPointInFront(ref endValue, MaxDis);
                            Skill.Caster.CacheTransform.DOMove(endValue, Duration);
                        }
                    }
                    break;
                case MoveType.MOVE_BACK:
                    {
                        Vector3 endValue = Skill.Caster.Pos - Skill.Caster.Dir * MaxDis;        
                        Skill.Caster.CheckMoveEndPointInBack(ref endValue, MaxDis);
                        Skill.Caster.CacheTransform.DOMove(endValue, Duration);
                    }
                    break;
            }

            return true;
        }


    }
}

