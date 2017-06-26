using UnityEngine;
using System.Collections;
using ACT;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/动作节点", Label = "TryCastSkill")]
    public class TryCastSkill : AITask
    {
        public override ENST OnExecute()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner == null)
            {
                return ENST.FAILURE;
            }
            ActSkill skill = aiOwner.Skill.SelectSkillByEnemy(aiOwner.Target);
            if (skill == null)
            {
                return ENST.FAILURE;
            }
            if (aiOwner.Command.Get<CommandUseSkill>().Update(skill.ID).Do() == Resp.TYPE_YES)
            {
                return ENST.SUCCESS;
            }
            else
            {
                return ENST.FAILURE;
            }
        }
    }
}

