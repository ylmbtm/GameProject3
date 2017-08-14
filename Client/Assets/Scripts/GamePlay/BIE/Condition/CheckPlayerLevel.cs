using UnityEngine;
using System.Collections;
using System.Xml;

namespace BIE
{
    public class CheckPlayerLevel : CheckCondition
    {
        public ECompare Compare = ECompare.EQ;
        public int      Level;

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.Compare = os.GetEnum<ECompare>("Compare");
            this.Level   = os.GetInt32("Level");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "Compare", Compare.ToString());
            DCFG.Write(doc, os, "Level",   Level);
        }

        public override bool Check()
        {
            switch (Compare)
            {
                case ECompare.EQ:
                    return GTGlobal.Main.Level == Level;
                case ECompare.GT:
                    return GTGlobal.Main.Level > Level;
                case ECompare.LT:
                    return GTGlobal.Main.Level < Level;
                case ECompare.GE:
                    return GTGlobal.Main.Level >= Level;
                case ECompare.LE:
                    return GTGlobal.Main.Level <= Level;
                default:
                    return true;
            }
        }

    }
}