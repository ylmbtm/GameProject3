using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActAudioLoop : ActAudio
    {
        public AudioSource TempSource
        {
            get; private set;
        }

        public ActAudioLoop()
        {
            EventType = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            this.TempSource = GTAudioManager.Instance.PlayEffectAudio(ClipName, Volume, Pitch, true);
            return true;
        }

        protected override void End()
        {
            GTAudioManager.Instance.EnqueueEffectAudio(TempSource);
            this.TempSource = null;
            base.End();
        }

        protected override void Exit()
        {
            GTAudioManager.Instance.EnqueueEffectAudio(TempSource);
            this.TempSource = null;
            base.Exit();
        }

        public override void Clear()
        {
            GTAudioManager.Instance.EnqueueEffectAudio(TempSource);
            this.TempSource = null;
            base.Clear();
        }
    }
}

