using UnityEngine;
using System.Collections;
using System.Xml;

namespace BIE
{
    public class CheckMonsterHP : CheckCondition
    {
        public override void Read(XmlElement os)
        {
            base.Read(os);
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
        }

        public override bool Check()
        {
            return base.Check();
        }
    }
}

