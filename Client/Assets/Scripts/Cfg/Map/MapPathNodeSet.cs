using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;
using System.Collections.Generic;

namespace CFG
{
    public class MapPathNodeSet : MapElement
    {
        public EPathType         Type      = EPathType.Linear;
        public List<MapPathNode> PathNodes = new List<MapPathNode>();

        public override void Read(XmlElement os)
        {
            this.Id         = os.GetInt("Id");
            this.Type       = os.GetEnum<EPathType>("Type");
            this.PathNodes  = ReadList<MapPathNode>(os);
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "Id",        this.Id);
            DCFG.Write(doc, os, "Type",      this.Type.ToString());
            DCFG.Write(doc, os, "PathNodes", this.PathNodes);
        }
    }
}

