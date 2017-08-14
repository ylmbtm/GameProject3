using UnityEngine;
using System.Collections;

namespace ACT
{
    public class MoverFly : MoverBase
    {
        public Character      Target
        {
            get; private set;
        }

        protected ActFlyObj      mFlyObj;
        protected float          mMoveSpeed;
        protected Vector3        mMoveDir;
        protected AnimationCurve mMoveSpeedCurve;
        protected float          mMoveStartTime;
        protected float          mMoveStartSpeed;
        protected float          mMovePastTime;

        public MoverFly(ActFlyObj obj, Character target, HitCallbackFly onTriggerEnter)
        {
            this.Target = target;
            this.mMoveSpeed = obj.MoveSpeed;
            this.mMoveStartSpeed = obj.MoveSpeed;
            this.mMoveSpeedCurve = GetCurve(obj.MoveSpeedCurve);
            this.mMoveStartTime = Time.time;
            this.Load(obj, onTriggerEnter);
        }

        public void Load(ActFlyObj obj, HitCallbackFly hitCallback)
        {
            mFlyObj = obj;
            if (obj.World)
            {
                Transform p = obj.Skill.Caster.Avatar.GetBindTransform(obj.CasterBind);
                mUnit = GTWorld.Instance.Ect.LoadEffect(obj.ID, 0, obj.Retain);
                mUnit.CacheTransform.parent = p;
                mUnit.CacheTransform.localPosition = obj.Offset;
                mUnit.CacheTransform.localEulerAngles = obj.Euler;
                mUnit.CacheTransform.parent = null;
                mUnit.CacheTransform.localScale = obj.Scale;
                mUnit.CacheTransform.localEulerAngles = obj.Euler + obj.Skill.Caster.Euler;
            }
            else
            {
                Transform p = obj.Skill.Caster.Avatar.GetBindTransform(obj.CasterBind);
                mUnit = GTWorld.Instance.Ect.LoadEffect(obj.ID, 0, GTTransform.Create(obj.Offset, obj.Euler, obj.Scale), p, obj.Retain);
            }
            mUnit.TriggerEnter = (other) => { hitCallback(this, other); };
            mIsLoaded = true;
            CacheTransform = mUnit.CacheTransform;
        }

        public override void Execute()
        {
            base.Execute();
            mMovePastTime = Time.time - mMoveStartTime;
            if (mMoveSpeedCurve != null)
            {    
                mMoveSpeed = mMoveSpeedCurve.Evaluate(mMovePastTime) * this.mMoveStartSpeed;
            }
            if (mUnit != null && mFlyObj != null && mMovePastTime >= mFlyObj.Duration)
            {
                mUnit.Release();
                mUnit = null;
            }
        }
    }
}

