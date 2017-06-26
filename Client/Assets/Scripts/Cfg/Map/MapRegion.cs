using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;
using System.Collections.Generic;

namespace CFG
{
    public class MapRegion : MapElement
    {
        public string                    Name         = string.Empty;
        public Vector3                   Pos          = Vector3.zero;
        public Vector3                   Euler        = Vector3.zero;
        public Vector3                   Scale        = Vector3.one;
        public bool                      AllowRide    = true;
        public bool                      AllowPK      = true;
        public bool                      AllowTrade   = true;
        public bool                      AllowFight   = true;
        public bool                      AwakeActive  = false;
        public List<MapEvent>            Events       = new List<MapEvent>();

        public override void Read(XmlElement os)
        {
            this.Id            = os.GetInt("Id");
            this.Name          = os.GetString("Name");
            this.Pos           = os.GetVector3("Pos");
            this.Euler         = os.GetVector3("Euler ");
            this.Scale         = os.GetVector3("Scale");
            this.AllowRide     = os.GetBool("AllowRide");
            this.AllowPK       = os.GetBool("AllowPK");
            this.AllowTrade    = os.GetBool("AllowTrade");
            this.AllowFight    = os.GetBool("AllowFight");
            this.AwakeActive   = os.GetBool("AwakeActive");
            this.Events        = ReadList<MapEvent>(os);
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Id",             this.Id);
            DCFG.Write(doc, os, "Name",           this.Name);
            DCFG.Write(doc, os, "Pos",            this.Pos);
            DCFG.Write(doc, os, "Euler",          this.Euler);
            DCFG.Write(doc, os, "Scale",          this.Scale);
            DCFG.Write(doc, os, "AllowRide",      this.AllowRide);
            DCFG.Write(doc, os, "AllowPK",        this.AllowPK);
            DCFG.Write(doc, os, "AllowTrade",     this.AllowTrade);
            DCFG.Write(doc, os, "AllowFight",     this.AllowFight);
            DCFG.Write(doc, os, "AwakeActive",    this.AwakeActive);
            DCFG.Write(doc, os, "Events",         this.Events);
        }
    }
}

