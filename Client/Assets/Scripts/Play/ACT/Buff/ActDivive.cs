using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActDivive : ActBuffItem
    {
        public override void Enter()
        {
            base.Enter();
            this.Carryer.IsDivive = true;
            this.Carryer.Skill.DelBuffByType(EBuffType.Debuff);
        }

        public override void Exit()
        {
            this.Carryer.IsDivive = false;
        }

        public override void Stop()
        {
            this.Carryer.IsDivive = false;
        }
    }
}
