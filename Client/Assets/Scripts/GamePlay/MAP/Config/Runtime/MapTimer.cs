using UnityEngine;
using System.Collections;
using System;
using System.Xml;
using System.Collections.Generic;

namespace CFG
{
    public class MapTimer : MapComponent
    {
        public float     TriggerTime   = 5;
        public Int32     TriggerLoops  = 1;
        public List<int> TriggerEvents = new List<int>();

        public override void Read(XmlElement os)
        {
            this.ID            = os.GetInt32("ID");
            this.TriggerTime   = os.GetFloat("TriggerTime");
            this.TriggerLoops  = os.GetInt32("TriggerLoops");
            this.TriggerEvents = os.GetList("TriggerEvents");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",            this.ID);
            DCFG.Write(doc, os, "TriggerTime",   this.TriggerTime);
            DCFG.Write(doc, os, "TriggerLoops",  this.TriggerLoops);
            DCFG.Write(doc, os, "TriggerEvents", this.TriggerEvents);
        }
    }
}