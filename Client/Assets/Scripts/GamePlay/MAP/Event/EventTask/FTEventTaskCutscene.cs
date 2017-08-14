using UnityEngine;
using System.Collections;
using System;

namespace MAP
{
    public class FTEventTaskCutscene : FTEventTask
    {
        public Int16 CutsceneID;

        public override void Trigger()
        {
            base.Trigger();
            GTWorld.Instance.Plt.Trigger(CutsceneID, Finish, Finish);
        }

        public override void Finish()
        {
            base.Finish();
        }
    }
}