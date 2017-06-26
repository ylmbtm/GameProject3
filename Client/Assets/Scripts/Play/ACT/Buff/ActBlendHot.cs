using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

namespace ACT
{
    public class ActBlendHot : ActBuffInterval
    {
        [SerializeField]
        public EHealType   Type          = EHealType.HP;
        [SerializeField]
        public List<float> TickPercents  = new List<float>();
        [SerializeField]
        public List<Int32> TickFixValues = new List<int>();

        public float       CurrTickTime
        {
            get; private set;
        }

        public Int32       CurrTickIndex
        {
            get; private set;
        }

        public override void Enter()
        {
            base.Enter();
            if (PlayOnTrigger)
            {
                this.Trigger();
            }
            this.CurrTickTime = Time.realtimeSinceStartup;
        }

        public override void Execute()
        {
            if (Time.realtimeSinceStartup - this.CurrTickTime > TickInterval)
            {
                this.Trigger();
                this.CurrTickTime = Time.realtimeSinceStartup;
            }
        }

        public override void Overlay()
        {

        }

        void Trigger()
        {
            float tickPer = TickPercents.Count  >= (CurrTickIndex + 1) ? TickPercents[CurrTickIndex] : 0;
            Int32 tickFix = TickFixValues.Count >= (CurrTickIndex + 1) ? TickFixValues[CurrTickIndex] : 0;
            CharacterHelper.CalcHot(Type, Buff.Caster, Carryer, Buff.Name, tickPer, tickFix);
            this.CurrTickIndex++;
        }
    }
}
