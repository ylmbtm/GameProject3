using UnityEngine;
using System.Collections;
using System.Xml;
using System;
using System.Collections.Generic;

namespace CFG
{
    public class MapAreaMine : MapComponent
    {
        public int            MineID;
        public float          RebornCD;
        public Int32          RebornType;
        public int            DropMinCount;
        public int            DropMaxCount;
        public Vector3        Pos;
        public List<MapPoint> Points = new List<MapPoint>();

        public override void Read(XmlElement os)
        {
            this.ID            = os.GetInt32("ID");
            this.MineID        = os.GetInt32("MineID");
            this.DropMinCount  = os.GetInt32("DropMinCount");
            this.DropMaxCount  = os.GetInt32("DropMaxCount");
            this.RebornCD      = os.GetFloat("RebornCD");
            this.RebornType    = os.GetInt32("RebornType");
            this.Pos           = os.GetVector3("Pos");
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
            DCFG.Write(doc, os, "MineID",       this.MineID);
            DCFG.Write(doc, os, "DropMinCount", this.DropMinCount);
            DCFG.Write(doc, os, "DropMaxCount", this.DropMaxCount);
            DCFG.Write(doc, os, "RebornCD",     this.RebornCD);
            DCFG.Write(doc, os, "RebornType",   this.RebornType);
            DCFG.Write(doc, os, "Pos",          this.Pos);
            DCFG.Write(doc, os, "Points",       this.Points);
        }
    }
}