using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    [Serializable]
    public class TaskNpc : DCFG
    {
        public int          NpcID;
        public int          LifeTime;
        public TaskLocation Location = new TaskLocation();

        public override void Read(XmlElement os)
        {
            this.NpcID    = os.GetInt32("NpcID");
            this.LifeTime = os.GetInt32("LifeTime");
            foreach (var current in GetChilds(os))
            {
                switch(current.Name)
                {
                    case "Location":
                        this.Location = ReadObj<TaskLocation>(current);
                        break;
                }
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "NpcID",    this.NpcID);
            DCFG.Write(doc, os, "LifeTime", this.LifeTime);
            DCFG.Write(doc, os, "Location", this.Location);
        }
    }
}

