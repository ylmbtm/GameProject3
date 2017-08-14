using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActAudio : ActItem
    {
        [SerializeField]
        public string ClipName = string.Empty;
        [SerializeField]
        public float  Volume = 1;
        [SerializeField]
        public float  Pitch = 1;


        public AudioSource TempSource
        {
            get; private set;
        }

        public ActAudio()
        {
            EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            this.TempSource = GTAudioManager.Instance.PlayEffectAudio(ClipName, Volume, Pitch, false);
            return true;
        }

        public override void Stop()
        {
            GTAudioManager.Instance.EnqueueEffectAudio(TempSource);
            this.TempSource = null;
            base.Stop();
        }
    }
}

