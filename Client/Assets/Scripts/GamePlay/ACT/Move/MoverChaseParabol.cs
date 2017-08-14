using UnityEngine;
using System.Collections;

namespace ACT
{
    public class MoverChaseParabol : MoverFly
    {
        public MoverChaseParabol(ActMissile obj, Character target, HitCallbackFly onTriggerEnter) :
            base(obj, target, onTriggerEnter)
        {

        }

        public override void Execute()
        {
            base.Execute();
            if (Target == null)
            {
                this.mMoveDir = CacheTransform.forward;
                Vector3 dir = this.mMoveDir;
                dir.Normalize();
                CacheTransform.position += dir * Time.deltaTime * mMoveSpeed;
            }
            else
            {
                if (Target.IsDead() || Target.IsStealth)
                {
                    this.Release();
                }
                else
                {
                    Vector3 pos = Target.Pos + new Vector3(0, Target.Height / 2, 0);
                    Quaternion to = Quaternion.LookRotation(pos - CacheTransform.position, Vector3.up);
                    CacheTransform.rotation = Quaternion.Slerp(CacheTransform.rotation, to, Time.deltaTime * 4);
                    CacheTransform.Translate(CacheTransform.forward * Time.deltaTime * this.mMoveSpeed);
                }
            }
        }
    }
}

