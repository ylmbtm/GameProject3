using UnityEngine;
using System.Collections;
using System.Xml;
using System.Collections.Generic;

namespace CFG
{
    public class MapArea : MapComponent
    {
        public string    Name        = string.Empty;
        public int       Shape       = 0;
        public Vector3   Pos         = Vector3.zero;
        public Vector3   Scale       = Vector3.one;
        public bool      AllowRide   = false;
        public bool      AllowPK     = false;
        public bool      AllowTrade  = false;
        public List<int> Events      = new List<int>();

        public override void Read(XmlElement os)
        {
            this.ID                = os.GetInt32("ID");
            this.Shape             = os.GetInt32("Shape");
            this.Name              = os.GetString("Name");
            this.Pos               = os.GetVector3("Pos");
            this.Scale             = os.GetVector3("Scale");
            this.AllowRide         = os.GetBool("AllowRide");
            this.AllowPK           = os.GetBool("AllowPK");
            this.AllowTrade        = os.GetBool("AllowTrade");
            this.Events            = os.GetList("Events");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",          this.ID);
            DCFG.Write(doc, os, "Shape",       this.Shape);
            DCFG.Write(doc, os, "Name",        this.Name);
            DCFG.Write(doc, os, "Pos",         this.Pos);
            DCFG.Write(doc, os, "Scale",       this.Scale);
            DCFG.Write(doc, os, "AllowRide",   this.AllowRide);
            DCFG.Write(doc, os, "AllowPK",     this.AllowPK);
            DCFG.Write(doc, os, "AllowTrade",  this.AllowTrade);
            DCFG.Write(doc, os, "Events",      this.Events);
        }
    }
}