using UnityEngine;
using System.Collections;
using System.Xml;
using System.IO;
using System;

namespace CFG
{
    [Serializable]
    public class SubTriggerPlot : SubTaskBase
    {
        public int ID;

        public SubTriggerPlot()
        {
            Func = ETaskSubFuncType.TYPE_STORY;
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.ID    = os.GetInt32("ID");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "ID", ID);
        }
    }
}