using UnityEngine;
using System.Collections;
using CFG;

namespace MAP
{
    public class FTEventTriggerArea : FTEvent
    {
        public int AreaID;

        public override void Trigger()
        {
            Map.TriggerElement<FTArea>(AreaID);
        }

        public override void Import(DCFG cfg)
        {
            MapEvent data = cfg as MapEvent;
            LitJson.JsonData jd = new LitJson.JsonData(data.EventParams);
            this.ID             = data.ID;
            this.AreaID         = (int)jd["AreaID"];
        }

        public override DCFG Export()
        {
            MapEvent data       = new MapEvent();
            data.ID             = this.ID;
            data.EventType      = (int)EFTEventType.FTEventTriggerArea;
            LitJson.JsonData jd = new LitJson.JsonData();
            jd["AreaID"]        = AreaID;
            data.EventParams    = jd.ToJson();
            return data;
        }
    }
}