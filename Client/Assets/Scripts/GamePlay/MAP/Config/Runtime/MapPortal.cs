using UnityEngine;
using System.Collections;
using System.Xml;
using System;

namespace CFG
{
    public class MapPortal : MapComponent
    {
        public string  Name = string.Empty;
        public int     DestMapID;
        public Vector3 DestPos;
        public Vector3 Pos;
        public Vector3 Scale;
        public int     OpenConditionRelation;
        public int     OpenLevel;
        public int     OpenItemID;
        public int     OpenVIP;
 

        public override void Read(XmlElement os)
        {
            this.ID                    = os.GetInt32("ID");
            this.Name                  = os.GetString("Name");
            this.DestMapID             = os.GetInt32("DestMapID");
            this.DestPos               = os.GetVector3("DestPos");
            this.Pos                   = os.GetVector3("Pos");
            this.Scale                 = os.GetVector3("Scale");
            this.OpenConditionRelation = os.GetInt32("OpenConditionRelation");
            this.OpenLevel             = os.GetInt32("OpenLevel");
            this.OpenItemID            = os.GetInt32("OpenItemID");
            this.OpenVIP               = os.GetInt32("OpenVIP");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",                    this.ID);
            DCFG.Write(doc, os, "Name",                  this.Name);
            DCFG.Write(doc, os, "DestMapID",             this.DestMapID);
            DCFG.Write(doc, os, "DestPos",               this.DestPos);
            DCFG.Write(doc, os, "Pos",                   this.Pos);
            DCFG.Write(doc, os, "Scale",                 this.Scale);
            DCFG.Write(doc, os, "OpenConditionRelation", this.OpenConditionRelation);
            DCFG.Write(doc, os, "OpenLevel",             this.OpenLevel);
            DCFG.Write(doc, os, "OpenItemID",            this.OpenItemID);
            DCFG.Write(doc, os, "OpenVIP",               this.OpenVIP);
        }
    }
}