using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/动作节点", Label = "TryBackHome")]
    public class TryBackHome : AITask
    {
        public override ENST OnExecute()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner != null)
            {
                Resp resp = aiOwner.Command.Get<CommandMove>().Update(AIOwner.BornData.Pos, null).Do();
                return resp == Resp.TYPE_YES ? ENST.SUCCESS : ENST.FAILURE;
            }
            else
            {
                return ENST.FAILURE;
            }
        }
    }
}

