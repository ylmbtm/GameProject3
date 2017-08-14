using UnityEngine;
using System.Collections;
using System;
using System.Xml;
using System.Collections.Generic;

namespace CFG
{
    public class MapConfig : DCFG
    {
        public int                   MapID;
        public int                   MapType;
        public string                MapName         = string.Empty;
        public bool                  AllowRide       = true;
        public bool                  AllowPK         = true;
        public bool                  AllowTrade      = true;
        public List<MapArea>         MapAreas        = new List<MapArea>();
        public List<MapAreaMine>     MapAreaMines    = new List<MapAreaMine>();
        public List<MapAreaMonster>  MapAreaMonsters = new List<MapAreaMonster>();
        public List<MapBorn>         MapBorns        = new List<MapBorn>();
        public List<MapMutiPoint>    MapMutiPoints   = new List<MapMutiPoint>();
        public List<MapNpc>          MapNpcs         = new List<MapNpc>();
        public List<MapPortal>       MapPortals      = new List<MapPortal>();
        public List<MapPath>         MapPaths        = new List<MapPath>();
        public List<MapBarrier>      MapBarriers     = new List<MapBarrier>();
        public List<MapEvent>        MapEvents       = new List<MapEvent>();
        public List<MapTimer>        MapTimers       = new List<MapTimer>();
        public List<MapStep>         MapSteps        = new List<MapStep>();
 
        public override void Read(XmlElement os)
        {
            this.MapID           = os.GetInt32("MapID");
            this.MapType         = os.GetInt32("MapType");
            this.MapName         = os.GetString("MapName");
            this.AllowRide       = os.GetBool("AllowRide");
            this.AllowPK         = os.GetBool("AllowPK");
            this.AllowTrade      = os.GetBool("AllowTrade");
            foreach (var current in GetChilds(os))
            {
                switch (current.Name)
                {
                    case "MapAreas":
                        this.MapAreas        = ReadList<MapArea>(current);
                        break;
                    case "MapAreaMines":
                        this.MapAreaMines    = ReadList<MapAreaMine>(current);
                        break;
                    case "MapAreaMonsters":
                        this.MapAreaMonsters = ReadList<MapAreaMonster>(current);
                        break;
                    case "MapBorns":
                        this.MapBorns        = ReadList<MapBorn>(current);
                        break;
                    case "MapMutiPoints":
                        this.MapMutiPoints   = ReadList<MapMutiPoint>(current);
                        break;
                    case "MapNpcs":
                        this.MapNpcs         = ReadList<MapNpc>(current);
                        break;
                    case "MapPortals":
                        this.MapPortals      = ReadList<MapPortal>(current);
                        break;
                    case "MapPaths":
                        this.MapPaths        = ReadList<MapPath>(current);
                        break;
                    case "MapBarriers":
                        this.MapBarriers     = ReadList<MapBarrier>(current);
                        break;
                    case "MapEvents":
                        this.MapEvents       = ReadList<MapEvent>(current);
                        break;
                    case "MapTimers":
                        this.MapTimers       = ReadList<MapTimer>(current);
                        break;
                    case "MapSteps":
                        this.MapSteps        = ReadList<MapStep>(current);
                        break;
                }
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "MapID",             MapID);
            DCFG.Write(doc, os, "MapType",           MapType);
            DCFG.Write(doc, os, "MapName",           MapName);
            DCFG.Write(doc, os, "AllowRide",         AllowRide);
            DCFG.Write(doc, os, "AllowPK",           AllowPK);
            DCFG.Write(doc, os, "AllowTrade",        AllowTrade);
            DCFG.Write(doc, os, "MapAreas",          MapAreas);
            DCFG.Write(doc, os, "MapAreaMines",      MapAreaMines);
            DCFG.Write(doc, os, "MapAreaMonsters",   MapAreaMonsters);
            DCFG.Write(doc, os, "MapBorns",          MapBorns);
            DCFG.Write(doc, os, "MapMutiPoints",     MapMutiPoints);
            DCFG.Write(doc, os, "MapNpcs",           MapNpcs);
            DCFG.Write(doc, os, "MapPortals",        MapPortals);
            DCFG.Write(doc, os, "MapPaths",          MapPaths);
            DCFG.Write(doc, os, "MapBarriers",       MapBarriers);
            DCFG.Write(doc, os, "MapEvents",         MapEvents);
            DCFG.Write(doc, os, "MapTimers",         MapTimers);
            DCFG.Write(doc, os, "MapSteps",          MapSteps);
        }
    }
}