using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

namespace ACT
{
    public class ActDot : ActBuffInterval
    {
        [SerializeField]
        public EDamageType  Type                 = EDamageType.TYPE_PHYSICS;
        [SerializeField] 
        public float        TickPercent          = 1;
        [SerializeField]
        public Int32        TickFixValue         = 100;
        [SerializeField]
        public bool         IgnoreDefense        = false;

        public float        CurrTickTime
        {
            get; private set;
        }

        public override bool Check()
        {
            return Carryer.IsDivive == false;
        }

        public override void Enter()
        {
            if(PlayOnTrigger)
            {
                int o = Buff.CurOverlayNum;
                CharacterHelper.CalcDot(Buff.Caster, Carryer, Buff, Type, TickPercent * o, TickFixValue * o, IgnoreDefense);
            }
            this.CurrTickTime = Time.realtimeSinceStartup;
        }

        public override void Execute()
        {
            if (Time.realtimeSinceStartup - this.CurrTickTime > TickInterval)
            {
                int o = Buff.CurOverlayNum;
                CharacterHelper.CalcDot(Buff.Caster, Carryer, Buff, Type, TickPercent * o, TickFixValue * o, IgnoreDefense);
                this.CurrTickTime = Time.realtimeSinceStartup;
            }
        }

        public override void Refresh()
        {

        }
    }
}

