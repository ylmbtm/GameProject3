using UnityEngine;
using System.Collections;

namespace MAP
{
    public class FTEventTriggerSound : FTEvent
    {
        public string AssetPath = string.Empty;
        public float  Volume    = 1;
        public float  Pitch     = 1;

        public override void Trigger()
        {
            GTAudioManager.Instance.PlayEffectAudio(AssetPath, Volume, Pitch);
        }
    }
}