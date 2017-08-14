using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using UnityEngine;

namespace CFG
{
    [Serializable]
    public class TaskLocation : DCFG
    {
        public int     MapID;
        public Vector3 Pos;
        public Vector3 Euler;

        public override void Read(XmlElement os)
        {
            this.MapID    = os.GetInt32("MapID");
            this.Pos      = os.GetVector3("Pos");
            this.Euler    = os.GetVector3("Euler");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "MapID", this.MapID);
            DCFG.Write(doc, os, "Pos",   this.Pos);
            DCFG.Write(doc, os, "Euler", this.Euler);
        }
    }
}
