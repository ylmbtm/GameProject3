using UnityEngine;
using System.Collections;
using System.Xml;

namespace BIE
{
    public class CheckScene : CheckCondition
    {
        public int SceneID;

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.SceneID = os.GetInt32("SceneID");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "SceneID", SceneID);
        }

        public override bool Check()
        {
            return GTGlobal.CurCopyID == SceneID;
        }
    }
}