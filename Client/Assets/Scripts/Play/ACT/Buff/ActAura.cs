using UnityEngine;
using System.Collections;
using System;

namespace ACT
{
    public class ActAura : ActBuffInterval
    {
        [SerializeField]
        public float   Radius = 5;
        [SerializeField]
        public EAffect Affect = EAffect.Ally;
        [SerializeField]
        public string  Sound  = string.Empty;

        public float CurrTickTime
        {
            get; private set;
        }

        public override bool Check()
        {
            return true;
        }

        public override void Enter()
        {
            if (PlayOnTrigger)
            {
                this.Trigger();
            }
            this.CurrTickTime = Time.realtimeSinceStartup;
        }

        public override void Execute()
        {
            if (Time.realtimeSinceStartup - this.CurrTickTime > TickInterval)
            {
                this.Trigger();
                this.CurrTickTime = Time.realtimeSinceStartup;
            }
        }

        protected virtual void Trigger()
        {
            if (!string.IsNullOrEmpty(Sound))
            {
                GTAudioManager.Instance.PlayEffectAudio(Sound);
            }
        }
    }
}

