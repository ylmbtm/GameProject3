using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/动作节点", Label = "TryFindTargetNearest")]
    [NodeName("ff64cb", "<size=10>TryFindTargetNearest</size>")]
    public class TryFindTargetNearest : AITask
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
                Character enemy = aiOwner.FindEnemyNearest(aiOwner.Skill.WarnerDist);
                if (enemy != null)
                {
                    CharacterHelper.SetTarget(aiOwner, enemy);
                }
                return ENST.SUCCESS;
            }
        }
    }
}

