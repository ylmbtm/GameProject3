using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckHPPercent")]
    public class CheckTarHPPercent : AICondition
    {
        [NodeVariable]
        public float     Percent       = 0.5f;
        [NodeVariable]
        public EAITarget ApplyAITarget = EAITarget.TYPE_SELF;

        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            Character aiTarget = null;
            switch (ApplyAITarget)
            {
                case EAITarget.TYPE_SELF:
                    aiTarget = aiOwner;
                    break;

                case EAITarget.TYPE_TARGET:
                    aiTarget = aiOwner == null ? null : aiOwner.Target;
                    break;
                case EAITarget.TYPE_HOST:
                    aiTarget = aiOwner == null ? null : aiOwner.Host;
                    break;
            }
            if (aiTarget == null)
            {
                return false;
            }
            return (aiTarget.CurrAttr.HP * 1f) / aiTarget.CurrAttr.MaxHP > Percent;
        }
    }
}

