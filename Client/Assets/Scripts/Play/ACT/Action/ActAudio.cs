using UnityEngine;
using System.Collections;
using System;

namespace ACT
{
    public class ActAudio : ActItem
    {
        [SerializeField]
        public string ClipName  = string.Empty;
        [SerializeField]
        public float  Volume    = 1;
        [SerializeField]
        public float  Pitch     = 1;

        public ActAudio()
        {
            EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            GTAudioManager.Instance.PlayEffectAudio(ClipName, Volume, Pitch, false);
            return true;
        }
    }
}
