using UnityEngine;
using System.Collections;
using System;

namespace ACT
{
    public class ActAuraHeal : ActAura
    {
        [SerializeField]
        public EHealType Type     = EHealType.HP;
        [SerializeField]
        public float     Percent  = 0;
        [SerializeField]
        public Int32     FixValue = 0;

        protected override void Trigger()
        {
            base.Trigger();
            for (int i = 0; i < GTWorld.Characters.Count; i++)
            {
                Character defender = GTWorld.Characters[i];
                if (Carryer.Match(Affect, defender) && GTTools.GetHorizontalDistance(Carryer.Pos, defender.Pos) <= Radius)
                {
                    CharacterHelper.CalcHeal(Type, Carryer, defender, string.Empty, Percent, FixValue);
                }
            }
        }
    }
}

