using UnityEngine;
using System.Collections;

namespace ACT
{
    public class MoverLighting : MoverBase
    {
        private ActLighting         mLightObj;
        private HitCallbackLighting mHitCallback;

        public Character SrcCharacter
        {
            get; private set;
        }

        public Character DstCharacter
        {
            get; private set;
        }

        public MoverLighting(ActLighting obj, Character src, Character dst, HitCallbackLighting onTriggerEnter)
        {
            this.mLightObj = obj;
            this.SrcCharacter = src;
            this.DstCharacter = dst;
            this.mUnit = GTWorld.Instance.Ect.LoadEffect(obj.ID, 0, obj.Retain);
            this.CacheTransform = mUnit.CacheTransform;
            this.mUnit.Release(mLightObj.Duration);
            this.mIsLoaded = true;
            this.mHitCallback = onTriggerEnter;
        }

        public override void Execute()
        {
            base.Execute();
            Transform bindBoneSt = GTTools.GetBone(CacheTransform, "Bone001");
            Transform bindBoneEd = GTTools.GetBone(CacheTransform, "Bone002");
            if (SrcCharacter != null)
            {
                Transform bindBone = SrcCharacter.Avatar.GetBindTransform(mLightObj.CasterBind);
                bindBoneSt.position = bindBone != null ? bindBone.position : (SrcCharacter.Pos + Vector3.up * 1);
            }
            if (DstCharacter != null)
            {
                Transform bindBone = DstCharacter.Avatar.GetBindTransform(mLightObj.TargetBind);
                bindBoneEd.position = bindBone != null ? bindBone.position : (DstCharacter.Pos + Vector3.up * 1);
            }
            else
            {
                bindBoneEd.position = SrcCharacter.Pos + SrcCharacter.Dir * mLightObj.MaxDis;
            }
            if (mHitCallback != null)
            {
                mHitCallback.Invoke(this);
                mHitCallback = null;
            }
        }
    }
}

