using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTEventTriggerStep : FTEvent
    {
        public int StepID;

        public override void Trigger()
        {
            this.Map.TriggerElement<FTStep>(StepID);
        }
    }
}