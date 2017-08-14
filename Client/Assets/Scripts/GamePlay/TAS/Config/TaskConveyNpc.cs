using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Xml;

namespace CFG
{
    [Serializable]
    public class TaskConvoyNpc : DCFG
    {
        [SerializeField]
        public int NpcID;

        public override void Read(XmlElement os)
        {
            this.NpcID = os.GetInt32("NpcID");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            DCFG.Write(doc, os, "NpcID", NpcID);
        }
    }

}
