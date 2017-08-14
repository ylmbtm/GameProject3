using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTEventTriggerTimer : FTEvent
    {
        public int TimerID;

        public override void Trigger()
        {
            this.Map.TriggerElement<FTTimer>(TimerID);
        }
    }
}