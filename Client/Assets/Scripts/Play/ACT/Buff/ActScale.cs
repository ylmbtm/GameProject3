using UnityEngine;
using System.Collections;
using DG.Tweening;

namespace ACT
{
    public class ActScale : ActBuffItem
    {
        [SerializeField]
        public float Size;
        [SerializeField]
        public float FadeTime;

        public override bool Check()
        {
            return true;
        }

        public override void Enter()
        {
            float endValue = Size <= 0 ? 1 : Size;
            if (Duration > FadeTime)
            {
                Carryer.ObjTrans.DOScale(endValue, FadeTime);
            }
            else
            {
                Carryer.ObjTrans.localScale = Vector3.one * endValue;
            }
        }

        public override void Stop()
        {
            Carryer.CacheTransform.DOScale(1, FadeTime);
        }

        public override void Exit()
        {
            Carryer.CacheTransform.DOScale(1, FadeTime);
        }
    }
}