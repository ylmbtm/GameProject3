using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActFlyWeaponBind : ActFlyObj
    {
        [SerializeField]
        public bool NeedHideWeapon = true;

        public bool TempHideWeapon
        {
            get; private set;
        }

        protected override bool Trigger()
        {
            GTWorld.Instance.Act.MakeMove(this, Skill.Target, this.OnTriggerEnter);
            this.Skill.Caster.Avatar.SetWeaponActive(!NeedHideWeapon);
            return base.Trigger();
        }

        public override void Clear()
        {
            this.Skill.Caster.Avatar.SetWeaponActive(TempHideWeapon);
            base.Clear();
        }
    }
}

