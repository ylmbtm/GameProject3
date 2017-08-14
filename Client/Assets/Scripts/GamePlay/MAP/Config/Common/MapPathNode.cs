using UnityEngine;
using System.Collections;
using System;
using System.Xml;

namespace CFG
{
    public class MapPathNode : DCFG
    {
        public float   Time;
        public Vector3 Pos;
        public Vector3 EulerAngles;
        public Vector3 TangentPos;

        public override void Read(XmlElement os)
        {
            this.Time             = os.GetFloat("Time");
            this.Pos              = os.GetVector3("Pos");
            this.EulerAngles      = os.GetVector3("EulerAngles");
            this.TangentPos       = os.GetVector3("TangentPos");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Time",                 this.Time);
            DCFG.Write(doc, os, "Pos",                  this.Pos);
            DCFG.Write(doc, os, "EulerAngles",          this.EulerAngles);
            DCFG.Write(doc, os, "TangentPos",           this.TangentPos);
        }
    }
}