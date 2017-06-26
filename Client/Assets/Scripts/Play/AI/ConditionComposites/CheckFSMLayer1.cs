using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckFSMLayer1")]
    public class CheckFSMLayer1 : AICondition
    {
        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            return aiOwner == null ? false : aiOwner.IsFSMLayer1();
        }
    }
}