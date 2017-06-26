using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/动作节点", Label = "TryFollowHost")]
    public class TryFollowHost : AITask
    {
        public override ENST OnExecute()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner != null)
            {
                if (aiOwner.Host != null)
                {
                    Resp resp = aiOwner.Command.Get<CommandMove>().Update(aiOwner.FollowOffset + aiOwner.Host.Pos, null).Do();
                    return resp == Resp.TYPE_YES ? ENST.SUCCESS : ENST.FAILURE;
                }
                return ENST.SUCCESS;
            };
            return ENST.FAILURE;
        }
    }
}

