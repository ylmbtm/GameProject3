using UnityEngine;
using System.Collections;
using System;
using System.Xml;

namespace CFG
{
    public class MapEvent : MapComponent
    {
        public Int32  EventType   = 0;
        public string EventParams = string.Empty;

        public override void Read(XmlElement os)
        {
            this.ID          = os.GetInt32("ID");
            this.EventType   = os.GetInt32("EventType");
            this.EventParams = os.GetString("EventParams");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",          ID);
            DCFG.Write(doc, os, "EventType",   EventType);
            DCFG.Write(doc, os, "EventParams", EventParams);
        }
    }
}