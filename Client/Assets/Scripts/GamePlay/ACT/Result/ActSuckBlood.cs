using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

namespace ACT
{
    public class ActSuckBlood : ActResult
    {
        [SerializeField]
        public Int32         FixValue         = 100;
        [SerializeField]
        public float         Percent          = 1;

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
            return CharacterHelper.CalcSuckBlood(Skill.Caster, cc, FixValue, Percent);
        }
    }
}

