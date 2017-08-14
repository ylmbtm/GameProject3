using UnityEngine;
using System.Collections;
using System;
using System.Xml;

namespace BIE
{
    [Serializable]
    public class CheckCondition : DCFG
    {
        public EGuideCondition Type;

        public override void Read(XmlElement os)
        {
            this.Type = (EGuideCondition)os.GetInt32("Type");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Type", (int)this.Type);
        }

        public virtual  bool Check()
        {
            return true;
        }
    }
}