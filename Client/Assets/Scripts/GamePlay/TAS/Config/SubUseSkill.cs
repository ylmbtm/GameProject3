using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    [Serializable]
    public class SubUseSkill : SubTaskBase
    {
        public ESkillPos Pos;
        public int       Times;

        public SubUseSkill()
        {
            Func = ETaskSubFuncType.TYPE_USESKILL;
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.Pos   = (ESkillPos)os.GetInt32("Pos");
            this.Times = os.GetInt32("Times");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "Pos",   (int)Pos);
            DCFG.Write(doc, os, "Times",      Times);
        }
    }
}

