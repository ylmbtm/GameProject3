using UnityEngine;
using System.Collections;
using System.Xml;
using System.IO;

namespace CFG
{
    public class SubGather : SubTaskBase
    {
        public int          ID;
        public int          Count;
        public TaskLocation Location = new TaskLocation();

        public SubGather()
        {
            Func = ETaskSubFuncType.TYPE_GATHER;
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.ID    = os.GetInt32("ID");
            this.Count = os.GetInt32("Count");
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
            base.Write(doc, os);
            DCFG.Write(doc, os, "ID",       ID);
            DCFG.Write(doc, os, "Count",    Count);
            DCFG.Write(doc, os, "Location", Location);
        }
    }
}
