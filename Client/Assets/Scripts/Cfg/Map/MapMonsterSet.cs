using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;
using System.Collections.Generic;

namespace CFG
{
    public class MapMonsterSet : MapElement
    {
        public int         RegionID;
        public int         MonsterID;
        public int         MaxCount;
        public float       RebornCD;

        public override void Read(XmlElement os)
        {
            this.Id         = os.GetInt("Id");
            this.RegionID   = os.GetInt("RegionID");
            this.MonsterID  = os.GetInt("MonsterID");
            this.MaxCount   = os.GetInt("MaxCount");
            this.RebornCD   = os.GetFloat("RebornCD");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Id",        Id);
            DCFG.Write(doc, os, "RegionID",  RegionID);
            DCFG.Write(doc, os, "MonsterID", MonsterID);
            DCFG.Write(doc, os, "MaxCount",  MaxCount);
            DCFG.Write(doc, os, "RebornCD",  RebornCD);
        }
    }
}

