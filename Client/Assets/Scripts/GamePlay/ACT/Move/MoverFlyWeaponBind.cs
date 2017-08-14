using UnityEngine;
using System.Collections;

namespace ACT
{
    public class MoverFlyWeaponBind : MoverFly
    {
        public MoverFlyWeaponBind(ActFlyWeaponBind obj, Character target, HitCallbackFly onTriggerEnter) : base(obj, target, onTriggerEnter)
        {

        }
    }
}

