using UnityEngine;
using System.Collections;
using System;
using DG.Tweening;

namespace ACT
{
    public class ActShake : ActItem
    {
        [SerializeField]
        public float Strength   = 0.3f;
        [SerializeField]
        public Int32 Vibrato    = 20;
        [SerializeField]
        public float Randomness = 90;
        [SerializeField]
        public bool  Fadeout    = false;


        public ActShake()
        {
            this.EventType  = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            if(Skill.Caster.IsMain)
            {
                GTCameraManager.Instance.CameraCtrl.PlayShake(Duration, Strength, Vibrato, Randomness, Fadeout);      
            }
            return true;
        }

        protected override void End()
        {
            base.End();
            GTCameraManager.Instance.CameraCtrl.StopShake();
        }

        public override void Clear()
        {
            base.Clear();
            GTCameraManager.Instance.CameraCtrl.StopShake();
        }
    }
}