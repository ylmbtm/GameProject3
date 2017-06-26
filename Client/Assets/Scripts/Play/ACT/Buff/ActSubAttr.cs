using UnityEngine;
using System.Collections;
using System;

namespace ACT
{
    public class ActSubAttr : ActBuffItem
    {
        [SerializeField]
        public EAttr AttrID           = EAttr.AP;
        [SerializeField]
        public EAttr AttrPercentRefer = EAttr.AP;
        [SerializeField]
        public float Percent          = 0;
        [SerializeField]
        public int   FixValue         = 0;

        public float CurrPercent
        {
            get; private set;
        }

        public Int32 CurrFixValue
        {
            get; private set;
        }

        public override bool Check()
        {
            return Carryer.IsDivive == false;
        }

        public override void Enter()
        {
            int o = Buff.CurOverlayNum;
            this.CurrPercent = Percent * o;
            this.CurrFixValue = FixValue * o;
            CharacterHelper.CalcSubAttr(Carryer, AttrID, AttrPercentRefer, Percent * o, FixValue * o, true);
        }

        public override void Exit()
        {
            int o = Buff.CurOverlayNum;
            CharacterHelper.CalcSubAttr(Carryer, AttrID, AttrPercentRefer, Percent * o, FixValue * o, false);
        }

        public override void Stop()
        {
            int o = Buff.CurOverlayNum;
            CharacterHelper.CalcSubAttr(Carryer, AttrID, AttrPercentRefer, Percent * o, FixValue * o, false);
        }

        public override void Overlay()
        {
            CharacterHelper.CalcSubAttr(Carryer, AttrID, AttrPercentRefer, this.CurrPercent, this.CurrFixValue, false);
            int o = Buff.CurOverlayNum;
            this.CurrPercent = Percent * o;
            this.CurrFixValue = FixValue * o;
            CharacterHelper.CalcSubAttr(Carryer, AttrID, AttrPercentRefer, Percent * o, FixValue * o, true);
        }
    }
}
