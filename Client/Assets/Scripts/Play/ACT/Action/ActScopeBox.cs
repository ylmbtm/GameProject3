using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActScopeBox : ActScope
    {
        [SerializeField]
        public float L;
        [SerializeField]
        public float W;
        [SerializeField]
        public float H;

        protected override bool IsTouch(Character cc, Vector3 hitPoint, Vector3 hitDir)
        {
            if (cc == null || cc.CacheTransform == null)
            {
                return false;
            }
            if (H <= 0 || W <= 0 || H <= 0)
            {
                return false;
            }
            float y = hitPoint.y;
            float yMax = y + H;
            float yMin = y;

            if (cc.Pos.y + cc.Height <= yMin)
            {
                return false;
            }
            if (cc.Pos.y >= yMax)
            {
                return false;
            }
            Vector3 targetPos = cc.Pos;
            targetPos.y = 0;
            Vector3 centerPos = hitPoint;
            centerPos.y = 0;

            Vector3 forward = hitDir + Euler;
            float angle = Vector3.Angle(targetPos - centerPos, forward);
            if (angle > 90)
            {
                return false;
            }
            float w = W / 2;
            float checkAngle = Mathf.Atan2(w, L) * Mathf.Rad2Deg;
            float distance = Vector3.Distance(targetPos, centerPos);
            if (angle <= checkAngle)
            {
                if (distance > L / Mathf.Cos(angle / Mathf.Rad2Deg))
                {
                    return false;
                }
            }
            else
            {
                if (distance > w / Mathf.Sin(angle / Mathf.Rad2Deg))
                {
                    return false;
                }
            }
            return true;
        }

        protected override void ShowWarning()
        {
            m_WarningPath = "Effect/10/skilltips_changfang";
            m_WarningObj = GTPoolManager.Instance.GetObject(m_WarningPath);
            if(ApplyByCenter)
            {
                m_WarningObj.transform.parent = ApplyCenter;
                m_WarningObj.transform.localPosition = Vector3.zero;
                m_WarningObj.transform.localEulerAngles = Vector3.zero;
            }
            else
            {
                m_WarningObj.transform.forward = ApplyHitDir;
                m_WarningObj.transform.localPosition = ApplyHitPoint;
            }
            m_WarningObj.transform.localScale = new Vector3(W, H, L);
        }
    }
}

