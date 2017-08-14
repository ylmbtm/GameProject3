using UnityEngine;
using System.Collections;
using System;
using System.Xml;

namespace CFG
{
    public class MapPoint : DCFG
    {
        public Vector3 Pos;
        public Vector3 EulerAngles;

        public override void Read(XmlElement os)
        {
            this.Pos         = os.GetVector3("Pos");
            this.EulerAngles = os.GetVector3("EulerAngles");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Pos",         this.Pos);
            DCFG.Write(doc, os, "EulerAngles", this.EulerAngles);
        }
    }
}