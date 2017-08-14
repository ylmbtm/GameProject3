using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActRay : ActItem
    {
        [SerializeField]
        public float   MaxDis            = 50;
        [SerializeField]
        public EBind   Bind              = EBind.Foot;
        [SerializeField]
        public EAffect Affect            = EAffect.Enem;
        [SerializeField]
        public int     HitEffectID       = 0;
        [SerializeField]
        public float   HitEffectDuration = 1;

        public ActRay()
        {
            EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            Vector3 origin = Skill.Caster.Avatar.GetBindPosition(Bind);
            RaycastHit hit;
            if (Physics.Raycast(origin, Skill.Caster.Dir, out hit, MaxDis))
            {
                Collider col = hit.collider;
                if (HitEffectID > 0)
                {
                    EffectData unit = GTWorld.Instance.Ect.LoadEffect(HitEffectID, 0, GTTransform.Create(hit.point, Vector3.zero), null, true);
                    unit.Release(HitEffectDuration);
                }
                CharacterView view = col.GetComponent<CharacterView>();
                if (view != null)
                {
                    if (Skill.Caster.Match(Affect, view.Owner))
                    {
                        for (int i = 0; i < Children.Count; i++)
                        {
                            Children[i].ApplyHitPoint = hit.point;
                            Children[i].ApplyHitDir = Vector3.up;
                            Children[i].ClearAttackList();
                            Children[i].AddInAttackList(view.Owner);
                        }
                        End();
                    }
                }
            }
            return base.Trigger();
        }
    }
}

