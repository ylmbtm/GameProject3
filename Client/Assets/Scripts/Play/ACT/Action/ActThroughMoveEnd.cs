using UnityEngine;
using System.Collections;
using DG.Tweening;

namespace ACT
{
    public class ActThroughMoveEnd : ActItem
    {
        [SerializeField]
        public string CachePosKey = "ThroughMove";

        public ActThroughMoveEnd()
        {
            EventType = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            if (Skill.Target != null)
            {
                Vector3 endValue = Skill.GetData<Vector3>(CachePosKey);
                Skill.Caster.CacheTransform.DOMove(endValue, Duration).OnComplete(OnMoveCompleted);
                return true;
            }
            GTCameraManager.Instance.CameraCtrl.isLockFollow = false;
            return false;
        }

        protected void OnMoveCompleted()
        {
            Skill.Caster.FaceTarget(0);
            GTCameraManager.Instance.CameraCtrl.isLockFollow = false;
        }
    }
}

