using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/动作节点", Label = "TryPursueTarget")]
    public class TryPursueTarget : AITask
    {
        public override ENST OnExecute()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner != null)
            {
                if (aiOwner.Target != null)
                {
                    Resp resp = aiOwner.Command.Get<CommandMove>().Update(aiOwner.Target, null).Do();
                    return resp == Resp.TYPE_YES ? ENST.SUCCESS : ENST.FAILURE;
                }
                return ENST.SUCCESS;
            };
            return ENST.FAILURE;
        }
    }
}

