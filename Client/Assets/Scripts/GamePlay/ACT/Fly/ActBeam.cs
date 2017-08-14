using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

namespace ACT
{
    public class ActBeam : ActObj
    {
        [SerializeField]
        public EBind   CasterBind      = EBind.Head;
        [SerializeField]
        public EBind   TargetBind      = EBind.Head;
        [SerializeField]
        public float   MaxDis          = 50;
        [SerializeField]
        public Int32   MaxCount        = 5;
        [SerializeField]
        public Int32   MaxBounceDis    = 5;
        [SerializeField]
        public EAffect Affect          = EAffect.Enem;

        private List<Character> mCurrAttackList = new List<Character>();

        public override void Clear()
        {
            base.Clear();
            mCurrAttackList.Clear();
        }

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

        protected override bool Trigger()
        {
            GTWorld.Instance.Act.MakeMove(this, Skill.Caster, Skill.Target, HitCallback);
            if (Skill.Target != null)
            {
                mCurrAttackList.Add(Skill.Target);
            }
            return base.Trigger();
        }

        protected void HitCallback(MoverBeam mover)
        {
            if(mover.DstCharacter == null)
            {
                return;
            }
            ActHelper.RunChildrenInNewTree(this, (tree) =>
            {
                for (int i = 0; i < tree.Children.Count; i++)
                {
                    tree.Children[i].ApplyHitPoint = mover.DstCharacter.Avatar.GetBindPosition(TargetBind);
                    tree.Children[i].ApplyHitDir = mover.CacheTransform.forward;
                    tree.Children[i].AddInAttackList(mover.DstCharacter);
                }
            });

            if (mCurrAttackList.Count >= MaxCount)
            {
                return;
            }

            List<Character> list = Skill.Caster.GetAffectCharacters(Affect, false);
            for (int i = 0; i < list.Count; i++)
            {
                Character cc = list[i];
                if (mCurrAttackList.Contains(cc))
                {
                    continue;
                }
                mCurrAttackList.Add(cc);
                if (GTTools.GetHorizontalDistance(cc.Pos, mover.DstCharacter.Pos) < MaxBounceDis)
                {
                    GTWorld.Instance.Act.MakeMove(this, mover.DstCharacter, cc, HitCallback);
                }
            }
        }
    }
}