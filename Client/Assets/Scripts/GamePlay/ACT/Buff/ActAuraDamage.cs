using UnityEngine;
using System.Collections;
using System;

namespace ACT
{
    public class ActAuraDamage : ActAura
    {
        [SerializeField]
        public EDamageType Type          = EDamageType.TYPE_PHYSICS;
        [SerializeField]
        public float       Percent       = 0;
        [SerializeField]
        public Int32       FixValue      = 0;
        [SerializeField]
        public bool        IgnoreDefense = false;

        protected override void Trigger()
        {
            base.Trigger();
            for (int i = 0; i < GTWorld.Characters.Count; i++)
            {
                Character defender = GTWorld.Characters[i];
                if (Carryer.Match(Affect, defender) && GTTools.GetHorizontalDistance(Carryer.Pos, defender.Pos) <= Radius)
                {
                    CharacterHelper.CalcDamage(Carryer, defender, null, Type, Percent, FixValue, IgnoreDefense);
                }
            }
        }
    }
}

