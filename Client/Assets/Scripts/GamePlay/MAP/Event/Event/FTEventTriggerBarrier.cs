using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTEventTriggerBarrier : FTEvent
    {
        public int BarrierID;

        public override void Trigger()
        {
            Map.TriggerElement<FTBarrier>(BarrierID);
        }
    }
}