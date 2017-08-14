using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActMissile : ActFlyObj
    {
        protected override bool Trigger()
        {
            GTWorld.Instance.Act.MakeMove(this, Skill.Target, OnTriggerEnter);
            return base.Trigger();
        }
    }
}