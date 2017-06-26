using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Xml;
using System.IO;

namespace CFG
{
    public class MapWave : MapElement
    {
        public int                       Index;
        public string                    IndexName = string.Empty;
        public float                     Delay;
        public EMonsterWaveSpawn         Spawn;
        public int                       AddBuffID;
        public List<MapMonster>      Monsters = new List<MapMonster>();


        public override void Read(XmlElement os)
        {
            this.Id              = os.GetInt("Id");
            this.Index           = os.GetInt("Index");
            this.IndexName       = os.GetString("IndexName");
            this.Delay           = os.GetFloat("Delay");
            this.Spawn           = (EMonsterWaveSpawn)os.GetInt("Spawn");
            this.AddBuffID       = os.GetInt("AddBuffID");
            this.Monsters        = ReadList<MapMonster>(os);
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Id",           Id);
            DCFG.Write(doc, os, "Index",        Index);
            DCFG.Write(doc, os, "IndexName",    IndexName);
            DCFG.Write(doc, os, "Delay",        Delay);
            DCFG.Write(doc, os, "Spawn",   (int)Spawn);
            DCFG.Write(doc, os, "AddBuffID",    AddBuffID);
            DCFG.Write(doc, os, "Monsters",     Monsters);
        }
    }
}

