using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace ACT
{
    public class ActTrigger : ActResult
    {
        [SerializeField]
        public int           ID     = 0;
        [SerializeField]
        public EBind         Bind   = EBind.Body;
        [SerializeField]
        public Vector3       Offset = Vector3.zero;
        [SerializeField]
        public Vector3       Euler  = Vector3.zero;
        [SerializeField]
        public Vector3       Scale  = Vector3.one;
        [SerializeField]
        public bool          Retain = true;


        protected override bool Trigger()
        {
            base.Trigger();
            this.Do();
            return true;
        }

        protected override void Do()
        {
            switch (SelectTarget)
            {
                case ESelectTarget.TYPE_CASTER:
                    MakeResult(Skill.Caster);
                    break;
                case ESelectTarget.TYPE_TARGET:
                    MakeResult(Skill.Target);
                    break;
                case ESelectTarget.TYPE_TARGETLIST:
                    if (mTargetList != null)
                    {
                        for (int i = 0; i < mTargetList.Count; i++)
                        {
                            MakeResult(mTargetList[i]);
                        }
                    }
                    break;
            }
        }

        protected override bool MakeResult(Character cc)
        {
            EffectData unit = null;
            if (cc == null)
            {
                return false;
            }
            Transform p = cc.Avatar.GetBindTransform(Bind);
            unit = GTWorld.Instance.Ect.LoadEffect(ID, 0, Retain);
            unit.CacheTransform.parent = p;
            unit.CacheTransform.localPosition = Offset;
            unit.CacheTransform.localEulerAngles = Euler;
            unit.CacheTransform.parent = null;

            unit.Release(Duration);
            ActTree item = Clone<ActTree>(this);
            item.Skill = this.Skill;
            item.StTime = 0;
            item.EdTime = 0;
            item.Clear();
            for (int i = 0; i < item.Children.Count; i++)
            {
                item.Children[i].ApplyCenter = unit.CacheTransform;
                item.Children[i].ApplyHitPoint = unit.CacheTransform.position;
            }
            GTWorld.Instance.Act.Run(item);
            return true;
        }
    }
}