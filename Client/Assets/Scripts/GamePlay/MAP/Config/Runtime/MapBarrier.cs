using UnityEngine;
using System.Collections;
using System.Xml;

namespace CFG
{
    public class MapBarrier : MapComponent
    {
        public float   H     = 7;
        public float   W     = 2;
        public float   L     = 1;
        public Vector3 Pos   = Vector3.zero;
        public float   Face  = 0;

        public override void Read(XmlElement os)
        {
            this.ID    = os.GetInt32("ID");
            this.L     = os.GetFloat("L");
            this.W     = os.GetFloat("W");
            this.H     = os.GetFloat("H");
            this.Pos   = os.GetVector3("Pos");
            this.Face  = os.GetFloat("Face");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",       this.ID);
            DCFG.Write(doc, os, "L",        this.L);
            DCFG.Write(doc, os, "W",        this.W);
            DCFG.Write(doc, os, "H",        this.H);
            DCFG.Write(doc, os, "Pos",      this.Pos);
            DCFG.Write(doc, os, "Face",     this.Face);
        }
    }
}