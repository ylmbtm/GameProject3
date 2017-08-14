using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

namespace CFG
{
    public class MapStep : MapComponent
    {
        public string      StepName   = string.Empty;
        public List<int>   StepEvents = new List<int>(); 

        public override void Read(XmlElement os)
        {
            this.ID         = os.GetInt32("ID");
            this.StepName   = os.GetString("StepName");
            this.StepEvents = os.GetList("StepEvents");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",         this.ID);
            DCFG.Write(doc, os, "StepName",   this.StepName);
            DCFG.Write(doc, os, "StepEvents", this.StepEvents);
        }
    }
}