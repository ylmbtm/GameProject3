using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/动作节点", Label = "TryClearTarget")]
    public class TryClearTarget : AITask
    {
        public override ENST OnExecute()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner != null)
            {
                CharacterHelper.ClearTarget(aiOwner);
                return ENST.SUCCESS;
            };
            return ENST.FAILURE;
        }
    }
}

