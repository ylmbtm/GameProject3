using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

namespace ACT
{
    public class ActBlendDot : ActBuffInterval
    {
        [SerializeField]
        public EDamageType Type          = EDamageType.TYPE_PHYSICS;
        [SerializeField] 
        public List<float> TickPercents  = new List<float>();
        [SerializeField]
        public List<Int32> TickFixValues = new List<Int32>();
        [SerializeField]
        public bool        IgnoreDefense = false;

        public float       CurrTickTime
        {
            get; private set;
        }

        public Int32       CurrTickIndex
        {
            get; private set;
        }

        public override bool Check()
        {
            return Carryer.IsDivive == false;
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
            float tickPer = TickPercents.Count >= (CurrTickIndex + 1) ? TickPercents[CurrTickIndex] : 0;
            Int32 tickFix = TickFixValues.Count >= (CurrTickIndex + 1) ? TickFixValues[CurrTickIndex] : 0;
            CharacterHelper.CalcDot(Buff.Caster, Carryer, Buff, Type, tickPer, tickFix, IgnoreDefense);
            this.CurrTickIndex++;
        }
    }
}

