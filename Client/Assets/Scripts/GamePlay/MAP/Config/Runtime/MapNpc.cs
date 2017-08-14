using UnityEngine;
using System.Collections;
using System.Xml;

namespace CFG
{
    public class MapNpc : MapComponent
    {
        public int     NpcID;
        public Vector3 Pos;
        public float   Face;

        public override void Read(XmlElement os)
        {
            this.ID        = os.GetInt32("ID");
            this.NpcID     = os.GetInt32("NpcID");
            this.Pos       = os.GetVector3("Pos");
            this.Face      = os.GetFloat("Face");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "ID",    this.ID);
            DCFG.Write(doc, os, "NpcID", this.NpcID);
            DCFG.Write(doc, os, "Pos",   this.Pos);
            DCFG.Write(doc, os, "Face",  this.Face);
        }
    }
}