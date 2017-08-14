using UnityEngine;
using System.Collections;
using System;
using System.Xml;

namespace CFG
{
    public class MapWorldBoss : MapComponent
    {
        public Int32   BossID        = 0;
        public string  BornBroadcast = string.Empty;
        public string  OpenBroadcast = string.Empty;
        public string  DeadBroadcast = string.Empty;
        public string  FailBroadcast = string.Empty;
        public Vector3 Pos           = Vector3.zero;
        public float   Face          = 0;

        public override void Read(XmlElement os)
        {
            this.ID            = os.GetInt32("ID");
            this.BossID        = os.GetInt32("BossID");
            this.BornBroadcast = os.GetString("BornBroadcast");
            this.OpenBroadcast = os.GetString("OpenBroadcast");
            this.DeadBroadcast = os.GetString("DeadBroadcast");
            this.FailBroadcast = os.GetString("FailBroadcast");
            this.Pos           = os.GetVector3("Pos");
            this.Face          = os.GetFloat("Face");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",            this.ID);
            DCFG.Write(doc, os, "BossID",        this.BossID);
            DCFG.Write(doc, os, "BornBroadcast", this.BornBroadcast);
            DCFG.Write(doc, os, "OpenBroadcast", this.OpenBroadcast);
            DCFG.Write(doc, os, "DeadBroadcast", this.DeadBroadcast);
            DCFG.Write(doc, os, "FailBroadcast", this.FailBroadcast);
            DCFG.Write(doc, os, "Pos",           this.Pos);
            DCFG.Write(doc, os, "Face",          this.Face);
        }
    }
}