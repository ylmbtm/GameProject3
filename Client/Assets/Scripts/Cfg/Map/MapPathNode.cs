using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    public class MapPathNode : DCFG
    {
        public float   Time;
        public Vector3 Pos;
        public Vector3 Euler;

        public override void Read(XmlElement os)
        {
            this.Time       = os.GetFloat("Time");
            this.Pos        = os.GetVector3("Pos");
            this.Euler      = os.GetVector3("Euler");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Time",  this.Time);
            DCFG.Write(doc, os, "Pos",   this.Pos);
            DCFG.Write(doc, os, "Euler", this.Euler);
        }
    }
}

