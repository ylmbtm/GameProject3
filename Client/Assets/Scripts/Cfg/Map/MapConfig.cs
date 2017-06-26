using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;
using System.Collections.Generic;
using System.Linq;

namespace CFG
{
    public class MapConfig : DCFG
    {
        public int                   Id;
        public float                 Delay;
        public string                MapName         = string.Empty;
        public string                MapPath         = string.Empty;
        public bool                  AllowRide       = true;
        public bool                  AllowPK         = true;
        public bool                  AllowTrade      = true;
        public bool                  AllowFight      = true;
        public Vector3               CameraPos       = Vector3.zero;
        public Vector3               CameraEuler     = Vector3.zero;

        public List<MapBorn>         Borns           = new List<MapBorn>();
        public List<MapBarrier>      Barriers        = new List<MapBarrier>();
        public List<MapPortal>       Portals         = new List<MapPortal>();
        public List<MapRegion>       Regions         = new List<MapRegion>();
        public List<MapMonsterSet>   MonsterSets     = new List<MapMonsterSet>();
        public List<MapWaveSet>      WaveSets        = new List<MapWaveSet>();
        public List<MapNpc>          Npcs            = new List<MapNpc>();
        public List<MapMineSet>      MineSets        = new List<MapMineSet>();
        public List<MapObj>          Objs            = new List<MapObj>();
        public List<MapMonster>      Monsters        = new List<MapMonster>();

        public override void Read(XmlElement os)
        {
            this.Id             = os.GetInt("Id");
            this.Delay          = os.GetFloat("Delay");
            this.MapName        = os.GetString("MapName");
            this.MapPath        = os.GetString("MapPath");
            this.AllowRide      = os.GetBool("AllowRide");
            this.AllowPK        = os.GetBool("AllowPK");
            this.AllowTrade     = os.GetBool("AllowTrade");
            this.AllowFight     = os.GetBool("AllowFight");
            this.CameraPos      = os.GetVector3("CameraPos");
            this.CameraEuler    = os.GetVector3("CameraEuler");

            foreach (var current in GetChilds(os))
            {
                switch(current.Name)
                {
                    case "Borns":
                        this.Borns         = ReadList<MapBorn>(current);
                        this.A             = this.Borns.Find((item) => { return item.Camp == EBattleCamp.A; });
                        this.B             = this.Borns.Find((item) => { return item.Camp == EBattleCamp.B; });
                        this.C             = this.Borns.Find((item) => { return item.Camp == EBattleCamp.C; });
                        break;
                    case "Barriers":
                        this.Barriers      = ReadList<MapBarrier>(current);
                        break;
                    case "Portals":
                        this.Portals       = ReadList<MapPortal>(current);
                        break;
                    case "Regions":
                        this.Regions       = ReadList<MapRegion>(current);
                        break;
                    case "MonsterSets":
                        this.MonsterSets   = ReadList<MapMonsterSet>(current);
                        break;
                    case "WaveSets":
                        this.WaveSets      = ReadList<MapWaveSet>(current);
                        break;
                    case "Npcs":
                        this.Npcs          = ReadList<MapNpc>(current);
                        break;
                    case "MineSets":
                        this.MineSets      = ReadList<MapMineSet>(current);
                        break;
                    case "Objs":
                        this.Objs          = ReadList<MapObj>(current);
                        break;
                    case "Monsters":
                        this.Monsters      = ReadList<MapMonster>(current);
                        break;
                }
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Id",            Id);
            DCFG.Write(doc, os, "Delay",         Delay);
            DCFG.Write(doc, os, "MapName",       MapName);
            DCFG.Write(doc, os, "MapPath",       MapPath);
            DCFG.Write(doc, os, "AllowRide",     AllowRide);
            DCFG.Write(doc, os, "AllowPK",       AllowPK);
            DCFG.Write(doc, os, "AllowTrade",    AllowTrade);
            DCFG.Write(doc, os, "AllowFight",    AllowFight);
            DCFG.Write(doc, os, "Borns",         Borns);
            DCFG.Write(doc, os, "Barriers",      Barriers);
            DCFG.Write(doc, os, "Portals",       Portals);
            DCFG.Write(doc, os, "Regions",       Regions);
            DCFG.Write(doc, os, "MonsterSets",   MonsterSets);
            DCFG.Write(doc, os, "MineSets",      MineSets);
            DCFG.Write(doc, os, "WaveSets",      WaveSets);
            DCFG.Write(doc, os, "Npcs",          Npcs);
            DCFG.Write(doc, os, "Objs",          Objs);
            DCFG.Write(doc, os, "Monsters",      Monsters);
        }

        public MapBorn A { get; private set; }
        public MapBorn B { get; private set; }
        public MapBorn C { get; private set; }
    }
}

