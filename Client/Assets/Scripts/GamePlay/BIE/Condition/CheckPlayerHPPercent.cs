using UnityEngine;
using System.Collections;
using System.Xml;

namespace BIE
{
    public class CheckPlayerHPPercent : CheckCondition
    {
        public ECompare Compare = ECompare.EQ;
        public float    HPPercent;

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.Compare   = os.GetEnum<ECompare>("Compare");
            this.HPPercent = os.GetFloat("HPValue");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "Compare", Compare.ToString());
            DCFG.Write(doc, os, "HPValue", HPPercent);
        }

        public override bool Check()
        {
            if (GTWorld.Main == null)
            {
                return false;
            }
            int maxHP = GTWorld.Main.CurrAttr.MaxHP;
            int curHP = GTWorld.Main.CurrAttr.HP;
            float per = (curHP * 1f) / maxHP;
            switch (Compare)
            {
                case ECompare.EQ:
                    return per == HPPercent;
                case ECompare.GT:
                    return per >  HPPercent;
                case ECompare.LT:
                    return per <  HPPercent;
                case ECompare.GE:
                    return per >= HPPercent;
                case ECompare.LE:
                    return per <= HPPercent;
                default:
                    return true;
            }
        }
    }
}