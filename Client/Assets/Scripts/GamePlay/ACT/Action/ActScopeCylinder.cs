using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActScopeCylinder : ActScope
    {
        [SerializeField]
        public float MaxDis;    //最大有效距离
        [SerializeField]
        public int   HAngle;    //水平角度范围
        [SerializeField]
        public float Height;    //圆柱有效高度

        protected override bool IsTouch(Character cc, Vector3 hitPoint, Vector3 hitDir)
        {
            if (cc == null || cc.CacheTransform == null)
            {
                return false;
            }
            if (HAngle <= 0 || Height <= 0)
            {
                return false;
            }
            float y = hitPoint.y;
            float yMax = y + Height / 2;
            float yMin = y - Height / 2;
            if (cc.Pos.y + cc.Height <= yMin)
            {
                return false;
            }
            if (cc.Pos.y >= yMax)
            {
                return false;
            }
            Vector3 dirPos = Euler + hitDir;
            dirPos.y = 0;
            float radius = MaxDis + cc.Radius;
            if (GTTools.GetHorizontalDistance(hitPoint, cc.Pos) > radius)
            {
                return false;
            }
            Vector3 targetPos = cc.Pos;
            targetPos.y = 0;
            Vector3 centerPos = hitPoint;
            centerPos.y = 0;
            if (Vector3.Angle(targetPos - centerPos, dirPos) > HAngle / 2)
            {
                return false;
            }
            return true;
        }

        protected override void ShowWarning()
        {
            m_WarningPath = string.Empty;
            switch (HAngle)
            {
                case 60:
                    m_WarningPath = "Effect/10/skilltips_60du";
                    break;
                case 120:
                    m_WarningPath = "Effect/10/skilltips_120du";
                    break;
                case 180:
                    m_WarningPath = "Effect/10/skilltips_180du";
                    break;
                case 360:
                    m_WarningPath = "Effect/10/skilltips_yuan";
                    break;
            }
            if(string.IsNullOrEmpty(m_WarningPath))
            {
                return;
            }
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
            m_WarningObj.transform.localScale = new Vector3(MaxDis, Height, MaxDis);         
        }
    }
}

