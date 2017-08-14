using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActTaunt : ActResult
    {
        [SerializeField]
        public int ThreatValue = 0;

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
            cc.TauntBy(Skill.Caster, this.ThreatValue);
            return true;
        }
    }
}
