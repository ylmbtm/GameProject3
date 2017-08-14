using UnityEngine;
using System.Collections;
using DG.Tweening;

namespace ACT
{
    public class ActAnnular : ActFlyObj
    {
        [SerializeField]
        public bool NegativeDirection = false;

        protected override bool Trigger()
        {         
            if (World)
            {
                Vector3 rotatePoint = this.Skill.Caster.Avatar.GetBindPosition(CasterBind);
                GTWorld.Instance.Act.MakeMove(this, Skill.Target, OnTriggerEnter, rotatePoint, null);
            }
            else
            {
                Transform rotateTrans = this.Skill.Caster.CacheFixEuler;
                GTWorld.Instance.Act.MakeMove(this, Skill.Target, OnTriggerEnter, Vector3.zero, rotateTrans);
            }
            return base.Trigger();
        }
    }
}

