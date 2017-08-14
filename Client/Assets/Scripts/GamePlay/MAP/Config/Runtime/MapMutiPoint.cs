using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Xml;

namespace CFG
{
    public class MapMutiPoint : MapComponent
    {
        public List<MapPoint> Points = new List<MapPoint>();

        public override void Read(XmlElement os)
        {
            this.ID = os.GetInt32("ID");
            foreach (var current in GetChilds(os))
            {
                switch (current.Name)
                {
                    case "Points":
                        this.Points = ReadList<MapPoint>(current);
                        break;
                }
            }
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",     ID);
            DCFG.Write(doc, os, "Points", Points);
        }
    }
}