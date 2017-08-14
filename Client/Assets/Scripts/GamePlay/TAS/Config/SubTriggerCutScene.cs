using UnityEngine;
using System.Collections;
using System.Xml;
using System.IO;
using System;

namespace CFG
{
    [Serializable]
    public class SubTriggerCutscene : SubTaskBase
    {
        public int ID;

        public SubTriggerCutscene()
        {
            Func = ETaskSubFuncType.TYPE_CUTSCENE;
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.ID = os.GetInt32("ID");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "ID", ID);
        }
    }
}
