using UnityEngine;
using System.Collections;
using System;
using System.Xml;

namespace BIE
{
    public class GuideCutscene : GuideBase
    {
        public Int16 CutsceneID;

        public override void Enter()
        {
            base.Enter();
            GTWorld.Instance.Plt.Trigger(CutsceneID, Stop, Finish);
        }

        public override void Finish()
        {
            base.Finish();
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.CutsceneID = os.GetInt16("CutsceneID");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "CutsceneID", CutsceneID);
        }
    }
}
