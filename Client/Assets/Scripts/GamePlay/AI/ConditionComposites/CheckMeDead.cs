using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckMeDead")]
    public class CheckMeDead : AICondition
    {
        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner == null)
            {
                return false;
            }
            if (aiOwner.IsDead())
            {
                return true;
            }
            return false;
        }
    }
}

