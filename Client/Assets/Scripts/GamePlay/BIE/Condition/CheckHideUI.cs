using UnityEngine;
using System.Collections;
using System.Xml;

namespace BIE
{
    public class CheckHideUI : CheckCondition
    {
        public EWindowID WindowID;

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.WindowID = os.GetEnum<EWindowID>("WindowID");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "WindowID", WindowID.ToString());
        }

        public override bool Check()
        {
            GTWindow window = GTWindowManager.Instance.GetWindow(WindowID);
            return window == null ? false : window.IsVisable() == false;
        }
    }
}

