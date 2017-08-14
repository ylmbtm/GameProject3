using UnityEngine;
using System.Collections;

namespace ACT
{
    public class MoverChase : MoverFly
    {
        public MoverChase(ActMissile obj, Character target, HitCallbackFly onTriggerEnter) : base(obj, target, onTriggerEnter)
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
                    float x = Target.Pos.x;
                    float y = Target.Pos.y > CacheTransform.position.y ? Target.Pos.y : CacheTransform.position.y;
                    float z = Target.Pos.z;
                    this.mMoveDir = new Vector3(x, y, z) - CacheTransform.position;
                    Vector3 dir = this.mMoveDir;
                    dir.Normalize();
                    this.CacheTransform.rotation = Quaternion.LookRotation(dir);
                    Vector3 move = dir * Time.deltaTime * mMoveSpeed;
                    CacheTransform.position += move;
                }
            }
        }
    }
}

