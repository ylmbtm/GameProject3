using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActFlyObj : ActObj
    {
        [SerializeField]
        public EBind       CasterBind     = EBind.Head;
        [SerializeField]
        public EMoveToward MoveToward     = EMoveToward.MoveToTarget;
        [SerializeField]
        public float       MoveSpeed      = 30;
        [SerializeField]
        public EMoveType   MoveType       = 0;
        [SerializeField]
        public int         MoveSpeedCurve = 0;
        [SerializeField]
        public float       MoveHAngle     = 30;
        [SerializeField]
        public float       MoveVAngle     = 0;
        [SerializeField]
        public EAffect     Affect         = EAffect.Enem;
        [SerializeField]
        public bool        PassBody       = false;

        public override void Loop()
        {
            if (Status == EActStatus.INITIAL)
            {
                Begin();
            }
            if (PastTime < StTime)
            {
                return;
            }
            if (Trigger())
            {
                Exit();
            }
        }

        protected virtual void OnTriggerEnter(MoverFly mover, Collider other)
        {
            CharacterView view = other.GetComponent<CharacterView>();
            if (view != null)
            {
                switch (MoveToward)
                {
                    case EMoveToward.MoveToTarget:
                        if (Skill.Caster.Match(Affect, mover.Target) && view.Owner == mover.Target)
                        {
                            OnHitCallback(mover, other, view.Owner);
                        }
                        break;
                    case EMoveToward.MoveToForward:
                        if (Skill.Caster.Match(Affect, view.Owner))
                        {
                            OnHitCallback(mover, other, view.Owner);
                        }
                        break;
                }
            }
        }

        protected void OnHitCallback(MoverBase mover, Collider other, Character cc)
        {
            ActHelper.RunChildrenInNewTree(this, (tree) =>
            {
                for (int i = 0; i < tree.Children.Count; i++)
                {
                    tree.Children[i].ApplyHitPoint = other.ClosestPoint(mover.CacheTransform.position);
                    tree.Children[i].ApplyHitDir = mover.CacheTransform.forward;
                    tree.Children[i].AddInAttackList(cc);
                }
                if (PassBody == false)
                {
                    mover.Release();
                }
            });     
        }
    }
}