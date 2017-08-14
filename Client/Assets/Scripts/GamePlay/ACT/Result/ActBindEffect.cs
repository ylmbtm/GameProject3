using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActBindEffect : ActResult
    {
        [SerializeField]
        public int         ID     = 0;
        [SerializeField]
        public EBind       Bind   = EBind.Body;
        [SerializeField]
        public Vector3     Offset = Vector3.zero;
        [SerializeField]
        public Vector3     Euler  = Vector3.zero;
        [SerializeField]
        public Vector3     Scale  = Vector3.one;
        [SerializeField]
        public bool        World  = false;
        [SerializeField]
        public bool        Retain = true;

        protected override bool Trigger()
        {
            base.Trigger();
            this.Do();
            return true;
        }

        protected override bool MakeResult(Character cc)
        {
            if (cc == null)
            {
                return false;
            }
            EffectData unit = null;
            if (World)
            {
                Transform p = cc.Avatar.GetBindTransform(Bind);
                unit = GTWorld.Instance.Ect.LoadEffect(ID, 0, Retain);
                unit.CacheTransform.parent = p;
                unit.CacheTransform.localPosition = Offset;
                unit.CacheTransform.localEulerAngles = Euler;
                unit.CacheTransform.parent = null;
                unit.CacheTransform.localScale = Scale;
            }
            else
            {
                Transform p = cc.Avatar.GetBindTransform(Bind);
                unit = GTWorld.Instance.Ect.LoadEffect(ID, 0, GTTransform.Create(Offset, Euler, Scale), p, Retain);
            }
            unit.Release(Duration);
            return true;
        }
    }
}