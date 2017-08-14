using UnityEngine;
using System.Collections;

namespace ACT
{
    public class MoverBeam : MoverBase
    {
        private ActBeam         mBeamObj;
        private F3DBeam         mBeam;
        private HitCallbackBeam mHitCallback;

        public Character SrcCharacter
        {
            get; private set;
        }

        public Character DstCharacter
        {
            get; private set;
        }


        public MoverBeam(ActBeam obj, Character src, Character dst, HitCallbackBeam onTriggerEnter)
        {
            this.mBeamObj = obj;
            this.SrcCharacter = src;
            this.DstCharacter = dst;
            this.mUnit = GTWorld.Instance.Ect.LoadEffect(obj.ID, 0, obj.Retain);
            this.mBeam = this.mUnit.CacheTransform.GetComponent<F3DBeam>();
            this.CacheTransform = mUnit.CacheTransform;
            this.mUnit.Release(mBeamObj.Duration);
            this.mIsLoaded = true;
            this.mHitCallback = onTriggerEnter;
        }

        public override void Execute()
        {
            base.Execute();
            if (mBeam == null)
            {
                return;
            }
            Vector3 pos0 = SrcCharacter.Avatar.GetBindPosition(mBeamObj.CasterBind);
            Vector3 pos1 = Vector3.zero;
            if (DstCharacter == null || mBeamObj.TargetBind == EBind.None)
            {
                Vector3 dir = SrcCharacter.Dir;
                dir.Normalize();
                pos1 = pos0 + dir * mBeamObj.MaxDis;
            }
            else
            {
                pos1 = DstCharacter.Avatar.GetBindPosition(mBeamObj.TargetBind);
            }
            this.mBeam.transform.localPosition = pos0;
            this.mBeam.transform.LookAt(pos1);
            this.mBeam.MaxBeamLength = Vector3.Distance(pos1, pos0);
            if (mHitCallback != null)
            {
                mHitCallback.Invoke(this);
                mHitCallback = null;
            }
        }
    }
}