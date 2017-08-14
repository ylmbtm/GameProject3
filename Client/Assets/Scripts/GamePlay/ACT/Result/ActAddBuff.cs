using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace ACT
{
    public class ActAddBuff : ActResult
    {
        [SerializeField]
        public int           BuffID       = 0;
        [SerializeField]
        public float         Probably     = 0.5f;

        protected override bool Trigger()
        {
            base.Trigger();
            bool isTrigger = GTTools.IsTrigger(Probably);
            if(isTrigger)
            {
                this.Do();
            }
            return isTrigger;
        }

        protected override bool MakeResult(Character cc)
        {
            if (cc == null)
            {
                return false;
            }
            return CharacterHelper.CalcAddBuff(Skill.Caster, cc, BuffID);
        }
    }
}

