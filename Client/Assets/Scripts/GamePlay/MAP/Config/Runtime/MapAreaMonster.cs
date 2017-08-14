using UnityEngine;
using System.Collections;
using System;
using System.Xml;
using System.Collections.Generic;

namespace CFG
{
    public class MapAreaMonster : MapComponent
    {
        public Int32          MonsterID    = 0;
        public float          RebornCD     = 20;
        public Int32          RebornType   = 0;
        public Int32          PatrolType   = 0;
        public Int32          PatrolTypeID = 0;
        public Vector3        Pos          = Vector3.zero;
        public List<MapPoint> Points       = new List<MapPoint>();

        public override void Read(XmlElement os)
        {
            this.ID           = os.GetInt32("ID");
            this.MonsterID    = os.GetInt32("MonsterID");
            this.RebornCD     = os.GetFloat("RebornCD");
            this.RebornType   = os.GetInt32("RebornType");
            this.PatrolType   = os.GetInt32("PatrolType");
            this.PatrolTypeID = os.GetInt32("PatrolTypeID");
            this.Pos          = os.GetVector3("Pos");
            foreach (var current in GetChilds(os))
            {
                switch (current.Name)
                {
                    case "Points":
                        this.Points = ReadList<MapPoint>(current);
                        break;
                }
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",           this.ID);
            DCFG.Write(doc, os, "MonsterID",    this.MonsterID);
            DCFG.Write(doc, os, "RebornCD",     this.RebornCD);
            DCFG.Write(doc, os, "RebornType",   this.RebornType);
            DCFG.Write(doc, os, "PatrolType",   this.PatrolType);
            DCFG.Write(doc, os, "PatrolTypeID", this.PatrolTypeID);
            DCFG.Write(doc, os, "Pos",          this.Pos);
            DCFG.Write(doc, os, "Points",       this.Points);
        }
    }
}