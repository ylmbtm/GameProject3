using UnityEngine;
using System.Collections;
using Boo.Lang;

namespace ACT
{
    public class ActResult : ActItem
    {
        [SerializeField]
        public ESelectTarget SelectTarget = ESelectTarget.TYPE_CASTER;

        public ActResult()
        {
            EventType = EActEventType.Instant;
        }

        protected virtual void Do()
        {
            switch (SelectTarget)
            {
                case ESelectTarget.TYPE_CASTER:
                    MakeResult(Skill.Caster);
                    break;
                case ESelectTarget.TYPE_TARGET:
                    MakeResult(Skill.Target);
                    break;
                case ESelectTarget.TYPE_TARGETLIST:
                    if (mTargetList != null)
                    {
                        for (int i = 0; i <mTargetList.Count; i++)
                        {
                            MakeResult(mTargetList[i]);
                        }
                    }
                    break;
            }
        }

        protected virtual bool MakeResult(Character cc)
        {
            return false;
        }

        protected override void End()
        {
            base.End();
            this.ClearAttackList();
        }

        protected override void Exit()
        {
            base.Exit();
            this.ClearAttackList();
        }

        public override void Clear()
        {
            base.Clear();
            this.ClearAttackList();
        }
    }
}

