using UnityEngine;
using System.Collections;
using System.Xml;

namespace BIE
{
    public class CheckPlayerVip : CheckCondition
    {
        public ECompare Compare = ECompare.EQ;
        public int      VipLevel;

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.Compare  = os.GetEnum<ECompare>("Compare");
            this.VipLevel = os.GetInt32("VipLevel");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "Compare",  Compare.ToString());
            DCFG.Write(doc, os, "VipLevel", VipLevel);
        }

        public override bool Check()
        {
            switch (Compare)
            {
                case ECompare.EQ:
                    return GTGlobal.Main.VipLevel == VipLevel;
                case ECompare.GT:
                    return GTGlobal.Main.VipLevel >  VipLevel;
                case ECompare.LT:
                    return GTGlobal.Main.VipLevel <  VipLevel;
                case ECompare.GE:
                    return GTGlobal.Main.VipLevel >= VipLevel;
                case ECompare.LE:
                    return GTGlobal.Main.VipLevel <= VipLevel;
                default:
                    return true;
            }
        }
    }
}