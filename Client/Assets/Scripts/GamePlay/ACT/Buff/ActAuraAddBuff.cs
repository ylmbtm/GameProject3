using UnityEngine;
using System.Collections;
using System;

namespace ACT
{
    public class ActAuraAddBuff : ActAura
    {
        [SerializeField]
        public Int32 BuffID;

        protected override void Trigger()
        {
            base.Trigger();
            for (int i = 0; i < GTWorld.Characters.Count; i++)
            {
                Character defender = GTWorld.Characters[i];
                if (Carryer.Match(Affect, defender) && GTTools.GetHorizontalDistance(Carryer.Pos, defender.Pos) <= Radius)
                {
                    CharacterHelper.CalcAddBuff(Carryer, defender, BuffID);
                }
            }
        }
    }
}
