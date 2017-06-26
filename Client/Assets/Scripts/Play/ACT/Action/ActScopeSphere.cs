using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActScopeSphere : ActScope
    {
        [SerializeField]
        public float Radius = 1;

        protected override bool IsTouch(Character cc, Vector3 hitPoint, Vector3 dir)
        {
            if (cc == null || cc.CacheTransform == null)
            {
                return false;
            }
            if (Radius <= 0)
            {
                return false;
            }
            if (GTTools.GetHorizontalDistance(hitPoint, cc.Pos) < cc.Radius)
            {
                return true;
            }
            return false;
        }
    }
}

