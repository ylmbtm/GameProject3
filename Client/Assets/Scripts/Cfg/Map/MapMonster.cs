using UnityEngine;
using System.Collections;
using System.Xml;
using System.IO;

namespace CFG
{
    public class MapMonster : MapElement
    {
        public Vector3 Pos;
        public Vector3 Euler;

        public override void Read(XmlElement os)
        {
            this.Id    = os.GetInt("Id");
            this.Pos   = os.GetVector3("Pos");
            this.Euler = os.GetVector3("Euler");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Id", Id);
            DCFG.Write(doc, os, "Pos", Pos);
            DCFG.Write(doc, os, "Euler", Euler);
        }
    }
}
