using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    public class MapPortal: MapElement
    {
        public string     Name = string.Empty;
        public int        RegionID;
        public int        DestMapID;
        public Vector3    DestPos;
        public bool       DisplayText;
        public Vector3    Pos;
        public Vector3    Euler;
        public ECR        CR = ECR.AND;
        public int        OpenLevel;
        public int        OpenItemID;
        public int        OpenVIP;

        public override void Read(XmlElement os)
        {
            this.Id           = os.GetInt("Id");
            this.Name         = os.GetString("Name");
            this.RegionID     = os.GetInt("RegionID");
            this.DestMapID    = os.GetInt("DestMapID");
            this.DestPos      = os.GetVector3("RegionID");
            this.DisplayText  = os.GetBool("DisplayText");
            this.Pos          = os.GetVector3("Pos");
            this.Euler        = os.GetVector3("Euler");
            this.CR           = (ECR)os.GetInt("CR");
            this.OpenLevel    = os.GetInt("OpenLevel");
            this.OpenItemID   = os.GetInt("OpenItemID");
            this.OpenVIP      = os.GetInt("OpenVIP");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc,os,  "Id",          this.Id);
            DCFG.Write(doc, os, "Name",        this.Name);
            DCFG.Write(doc, os, "RegionID",    this.RegionID);
            DCFG.Write(doc, os, "DestMapID",   this.DestMapID);
            DCFG.Write(doc, os, "DestPos",     this.DestPos);
            DCFG.Write(doc, os, "DisplayText", this.DisplayText);
            DCFG.Write(doc, os, "Pos",         this.Pos);
            DCFG.Write(doc, os, "Euler",       this.Euler);
            DCFG.Write(doc, os, "CR",     (int)this.CR);
            DCFG.Write(doc, os, "OpenLevel",   this.OpenLevel);
            DCFG.Write(doc, os, "OpenItemID",  this.OpenItemID);
            DCFG.Write(doc, os, "OpenVIP",     this.OpenVIP);
        }
    }
}

