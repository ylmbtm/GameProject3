using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTEventTaskDelay : FTEventTask
    {
        public float Time;

        private GTTimer m_Timer;

        public override void Trigger()
        {
            base.Trigger();
            this.m_Timer = GTTimerManager.Instance.AddListener(Time, Finish);
        }

        public override void Finish()
        {
            base.Finish();
        }

        public override void Release()
        {
            GTTimerManager.Instance.DelListener(m_Timer);
            base.Release();
        }
    }
}
