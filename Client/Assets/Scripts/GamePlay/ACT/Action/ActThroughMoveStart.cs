using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActThroughMoveStart : ActItem
    {
        [SerializeField]
        public string CachePosKey = "ThroughMove";

        public ActThroughMoveStart()
        {
            EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            GTCameraManager.Instance.CameraCtrl.isLockFollow = true;
            Skill.SetData(CachePosKey, Skill.Caster.Pos);
            return base.Trigger();
        }
    }
}
