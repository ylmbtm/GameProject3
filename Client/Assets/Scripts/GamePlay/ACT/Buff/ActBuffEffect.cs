using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActBuffEffect : ActBuffItem
    {
        [SerializeField]
        public int     ID     = 0;
        [SerializeField]
        public EBind   Bind   = EBind.Body;
        [SerializeField]
        public Vector3 Offset = Vector3.zero;
        [SerializeField]
        public Vector3 Euler  = Vector3.zero;
        [SerializeField]
        public Vector3 Scale  = Vector3.one;
        [SerializeField]
        public bool    Retain = true;
        [SerializeField]
        public bool    World  = false;

        public EffectData Unit
        {
            get; protected set;
        }

        public override bool Check()
        {
            return true;
        }

        public override void Enter()
        {
            if (World)
            {
                Transform p = Carryer.Avatar.GetBindTransform(Bind);
                Unit = GTWorld.Instance.Ect.LoadEffect(ID, 0, Retain);
                Unit.CacheTransform.parent = p;
                Unit.CacheTransform.localPosition = Offset;
                Unit.CacheTransform.localEulerAngles = Euler;
                Unit.CacheTransform.parent = null;
                Unit.CacheTransform.localScale = Scale;
                Unit.CacheTransform.localEulerAngles = Euler + Carryer.Euler;
            }
            else
            {
                Transform p = Carryer.Avatar.GetBindTransform(Bind);
                Unit = GTWorld.Instance.Ect.LoadEffect(ID, 0, GTTransform.Create(Offset, Euler, Scale), p, Retain);
            }
        }

        public override void Stop()
        {
            if (Unit != null)
            {
                Unit.Release();
                Unit = null;
            }
        }

        public override void Exit()
        {
            if (Unit != null)
            {
                Unit.Release();
                Unit = null;
            }
        }
    }
}

