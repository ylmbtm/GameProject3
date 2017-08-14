using UnityEngine;
using System.Collections;
using System;

namespace ACT
{
    [Serializable]
    public class ActAnim : ActItem
    {
        [SerializeField]
        public string AnimName  = string.Empty;
        [SerializeField]
        public bool   IsLooped  = false;
        [SerializeField]
        public float  PlaySpeed = 1;

        public ActAnim()
        {
            EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            this.Skill.Caster.Action.Play(AnimName, null, IsLooped, PlaySpeed);
            return true;
        }
    }
}
