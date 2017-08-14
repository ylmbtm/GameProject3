using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTEventDestroyArea : FTEvent
    {
        public int AreaID;

        public override void Trigger()
        {
            Map.ReleaseElement<FTArea>(AreaID);
        }
    }
}