using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckTarDead")]
    public class CheckTarDead : AICondition
    {
        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner == null)
            {
                return false;
            }
            if (aiOwner.Target == null)
            {
                return false;
            }
            if (aiOwner.Target.IsDead())
            {
                return true;
            }
            return  false;
        }
    }

}
