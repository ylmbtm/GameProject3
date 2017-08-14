using UnityEngine;
using System.Collections;
using System;

namespace MAP
{
    public class FTEventTriggerEffect : FTEvent
    {
        public Int32 EffectID;
        public float EffectLifeTime;

        private EffectData m_EffectData;

        public override void Trigger()
        {
            base.Trigger();
            GTTransform data = GTTransform.Create(Pos, EulerAngles);
            m_EffectData = GTWorld.Instance.Ect.LoadEffect(EffectID, 0, data, null, false);
            m_EffectData.Release(EffectLifeTime);
        }
    }
}