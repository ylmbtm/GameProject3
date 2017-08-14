using UnityEngine;
using System.Collections;
using System;
using Protocol;
using System.Collections.Generic;

namespace ACT
{
    public class ActDamage : ActResult
    {
        [SerializeField]
        public EDamageType        Type              = EDamageType.TYPE_PHYSICS;
        [SerializeField]
        public float              Percent           = 1;
        [SerializeField]
        public Int32              FixValue          = 100;
        [SerializeField]
        public bool               IgnoreDefense     = false;

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
            return CharacterHelper.CalcDamage(Skill.Caster, cc, Skill, Type, Percent, FixValue, IgnoreDefense);
        }
    }
}

