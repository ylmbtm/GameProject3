using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    [Serializable]
    public class TaskCollectionItem : DCFG
    {
        public int             ID;
        public int             Count;
        public int             NpcID;
        public float           DropRate = 1;
        public TaskLocation    Location = new TaskLocation();

        public override void Read(XmlElement os)
        {
            this.ID        = os.GetInt32("ID");
            this.Count     = os.GetInt32("Count");
            this.NpcID     = os.GetInt32("NpcID");
            this.DropRate  = os.GetFloat("DropRate");
            foreach (var current in GetChilds(os))
            {
                switch (current.Name)
                {
                    case "Location":
                        this.Location = ReadObj<TaskLocation>(current);
                        break;
                }
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",          ID);
            DCFG.Write(doc, os, "Count",       Count);
            DCFG.Write(doc, os, "NpcID",       NpcID);
            DCFG.Write(doc, os, "DropRate",    DropRate);
            DCFG.Write(doc, os, "Location",    Location);
        }
    }
}

