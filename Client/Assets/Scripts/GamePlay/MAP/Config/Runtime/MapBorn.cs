using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    public class MapBorn : MapComponent
    {
        public Vector3        Pos    = Vector3.zero;
        public float          Face   = 0;

        public override void Read(XmlElement os)
        {
            this.ID         = os.GetInt32("ID");
            this.Pos        = os.GetVector3("Pos");
            this.Face       = os.GetFloat("Face");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc,os,  "ID",             ID);
            DCFG.Write(doc, os, "Pos",            Pos);
            DCFG.Write(doc, os, "Face",           Face);
        }
    }
}