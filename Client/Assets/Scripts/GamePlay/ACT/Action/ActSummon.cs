using UnityEngine;
using System.Collections;
using System;

namespace ACT
{
    public class ActSummon : ActItem
    {
        [SerializeField]
        public EActorType ActorType;
        [SerializeField]
        public Int32      Id;
        [SerializeField]
        public int        Count     = 1;
        [SerializeField]
        public float      MaxRadius = 5;
        [SerializeField]
        public float      MinRadius = 2;
        [SerializeField]
        public float      Interval = 0.5f;

        public ActSummon()
        {
            EventType = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            GTCoroutinueManager.Instance.StartCoroutine(DoSummon());
            return true;
        }

        IEnumerator DoSummon()
        {
            for (int i = 0; i < Count; i++)
            {
                yield return new WaitForSeconds(Interval);
                Vector3 bornPoint = GTTools.RandomOnCircle(Skill.Caster.Pos, MinRadius, MaxRadius);
                Vector3 bornEuler = Vector3.zero;
                GTTransform kt = GTTransform.Create(bornPoint, bornEuler);
                GTWorld.Instance.AddActor(Id, Skill.Caster.Camp, ActorType, kt, null, false);
            }
        }
    }
}
