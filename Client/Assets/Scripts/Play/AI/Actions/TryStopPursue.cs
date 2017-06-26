using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/动作节点", Label = "TryStopPursue")]
    public class TryStopPursue : AITask
    {
        public override ENST OnExecute()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner == null)
            {
                return ENST.FAILURE;
            }
            else
            {
                aiOwner.Move.Stop();
                return ENST.SUCCESS;
            }
        }
    }
}
