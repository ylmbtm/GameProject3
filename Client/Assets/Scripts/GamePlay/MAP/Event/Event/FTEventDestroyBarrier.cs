using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTEventDestroyBarrier : FTEvent
    {
        public int BarrierID;

        public override void Trigger()
        {
            Map.ReleaseElement<FTBarrier>(BarrierID);
        }
    }
}