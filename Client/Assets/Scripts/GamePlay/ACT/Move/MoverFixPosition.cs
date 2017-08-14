using UnityEngine;
using System.Collections;

namespace ACT
{
    public class MoverFixPosition : MoverFly
    {
        private float   m_HAngle;
        private float   m_VAngle;
        private Vector3 m_DestPosition;
        private float   m_DistanceToTarget;

        public MoverFixPosition(ActMissile obj, Character target, HitCallbackFly onTriggerEnter) :
            base(obj, target, onTriggerEnter)
        {
            this.m_HAngle = obj.MoveHAngle;
            this.m_VAngle = obj.MoveVAngle;
            this.m_DestPosition = target != null ? target.Avatar.GetBindPosition(EBind.Body) : CacheTransform.position + CacheTransform.forward * 10;
            this.m_DistanceToTarget = GTTools.GetHorizontalDistance(CacheTransform.position, m_DestPosition);
        }

        public override void Execute()
        {
            base.Execute();
            CacheTransform.LookAt(m_DestPosition);
            float distance = GTTools.GetHorizontalDistance(CacheTransform.position, m_DestPosition);
            if (m_VAngle != 0 || m_HAngle != 0)
            {
                float percent = Mathf.Min(1, distance / m_DistanceToTarget);
                float v = percent * m_VAngle;
                float h = percent * m_HAngle;
                CacheTransform.rotation *= Quaternion.Euler(Mathf.Clamp(v, -60, 60), Mathf.Clamp(h, -60, 60), 0);
            }
            CacheTransform.position += CacheTransform.forward * mMoveSpeed * Time.deltaTime;
        }
    }
}

