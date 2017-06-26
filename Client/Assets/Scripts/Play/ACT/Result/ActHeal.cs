using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

namespace ACT
{
    public class ActHeal : ActResult
    {
        [SerializeField]
        public EHealType     Type             = EHealType.HP; 
        [SerializeField]
        public float         Percent          = 1;
        [SerializeField]
        public Int32         FixValue         = 100;

        protected override bool Trigger()
        {
            base.Trigger();
            this.Do();
            return true;
        }

        protected override bool MakeResult(Character cc)
        {
            if (cc == null)
            {
                return false;
            }
            return CharacterHelper.CalcHeal(Type, Skill.Caster, cc, Skill.Name, Percent, FixValue);
        }
    }
}

