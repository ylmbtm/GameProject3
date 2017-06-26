using UnityEngine;
using System.Collections;

namespace ACT
{
    public class MoverFixDirection : MoverFly
    {
        public MoverFixDirection(ActMissile obj, Character target, HitCallbackFly onTriggerEnter) : 
            base(obj, target, onTriggerEnter)
        {

        }

        public override void Execute()
        {
            base.Execute();
            this.mMoveDir = CacheTransform.forward;
            Vector3 dir = this.mMoveDir;
            dir.Normalize();
            CacheTransform.position += dir * Time.deltaTime * mMoveSpeed;
        }
    }
}

