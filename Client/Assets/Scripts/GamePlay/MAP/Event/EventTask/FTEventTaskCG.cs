using UnityEngine;
using System.Collections;
using System;
using CFG;

namespace MAP
{
    public class FTEventTaskCG : FTEventTask
    {
        public Int16 VideoID = 1;

        public override void Trigger()
        {
            base.Trigger();
            GTWorld.Instance.Cut.Trigger(VideoID,
            (videoPlayer) =>
            {
               Finish();
            },
            (videoPlayer) =>
            {
               Finish();
            });
        }

        public override void Finish()
        {
            base.Finish();
        }

        public override void Import(DCFG cfg)
        {
            MapEvent data = cfg as MapEvent;
            data.ID = this.ID;
        }

        public override DCFG Export()
        {
            return base.Export();
        }
    }
}