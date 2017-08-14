using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActParticle : ActObj
    {
        [SerializeField]
        public string ParentPath = string.Empty;

        public ActParticle()
        {
            EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            GameObject go = GameObject.Find(ParentPath);
            if (go != null)
            {
                Unit = GTWorld.Instance.Ect.LoadEffect(ID, 0, GTTransform.Create(Offset, Euler, Scale), go.transform, Retain);
                Unit.Release(Duration);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
