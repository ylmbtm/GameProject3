using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActHideWeapon : ActItem
    {
        public ActHideWeapon()
        {
            this.EventType = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            Skill.Caster.Avatar.SetWeaponActive(false);
            return true;
        }

        protected override void End()
        {
            Skill.Caster.Avatar.SetWeaponActive(true);
            base.End();
        }

        public override void Clear()
        {
            Skill.Caster.Avatar.SetWeaponActive(true);
            base.Clear();
        }
    }
}

