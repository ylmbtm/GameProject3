using UnityEngine;
using System.Collections;
using System;
using System.Xml;

namespace BIE
{
    public class GuideCG : GuideBase
    {
        public Int16 VideoID = 1;

        public override void Enter()
        {
            base.Enter();
            GTWorld.Instance.Cut.Trigger(VideoID, 
            (videoPlayer)=>
            {
                Stop();
            },
            (videoPlayer) => 
            {
                Finish();
            });
        }

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.VideoID = os.GetInt16("VideoID");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "VideoID", VideoID);
        }
    }
}