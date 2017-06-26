using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActLog : ActItem
    {
        [SerializeField]
        public string Text = string.Empty;

        public ActLog()
        {
            this.EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            Debug.LogError(Text);
            return true;
        }
    }
}


