using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActScopeTriangle : ActScope
    {
        [SerializeField]
        public Vector3 A;
        [SerializeField]
        public Vector3 B;
        [SerializeField]
        public Vector3 C;
        [SerializeField]
        public Vector3 H;

        protected override bool IsTouch(Character cc, Vector3 hitPoint, Vector3 dir)
        {
            return base.IsTouch(cc, hitPoint, dir);
        }

        protected float Cross(Vector3 p1, Vector3 p2, Vector3 p)
        {
            return (p2.x - p1.x) * (p.z - p1.z) - (p2.z - p1.z) * (p.x - p1.x);
        }

        protected bool ToLeft(Vector3 p1, Vector3 p2, Vector3 p)
        {
            return Cross(p1, p2, p) > 0;
        }

        protected bool InTriangle(Vector3 p, Vector3 a, Vector3 b, Vector3 c)
        {
            bool res = ToLeft(a, b, p);
            if (res != ToLeft(b, c, p))
                return false;
            if (res != ToLeft(c, a, p))
                return false;
            if (Cross(a, b, c) == 0)
                return false;
            return true;
        }
    }
}

