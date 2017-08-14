using UnityEngine;
using System.Collections;
using System;

namespace ACT
{
    public class ActHot : ActBuffInterval
    {
        [SerializeField]
        public EHealType Type     = EHealType.HP;
        [SerializeField]
        public float     Percent  = 1;
        [SerializeField]
        public Int32     FixValue = 100;

        public float CurrTickTime
        {
            get; private set;
        }

        public override bool Check()
        {
            return true;
        }

        public override void Enter()
        {
            if (PlayOnTrigger)
            {
                int o = Buff.CurOverlayNum;
                CharacterHelper.CalcHot(Type, Buff.Caster, Carryer, Buff.Name, Percent * o, FixValue * o);
            }
            this.CurrTickTime = Time.realtimeSinceStartup;
        }

        public override void Execute()
        {
            if (Time.realtimeSinceStartup - this.CurrTickTime > TickInterval)
            {
                int o = Buff.CurOverlayNum;
                CharacterHelper.CalcHot(Type, Buff.Caster, Carryer, Buff.Name, Percent * o, FixValue * o);
                this.CurrTickTime = Time.realtimeSinceStartup;
            }
        }

        public override void Refresh()
        {

        }
    }
}
