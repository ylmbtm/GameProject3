using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTEventDestroyTimer : FTEvent
    {
        public int TimerID;

        public override void Trigger()
        {
            this.Map.ReleaseElement<FTTimer>(TimerID);
        }
    }
}