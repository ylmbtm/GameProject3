using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "装饰节点", Label = "时间节点")]
    [NodeDesc("在指定的时间内，持续调用其子节点")]
    [NodeName("05CBF2", "Time")]
    public class BTTime : BTDecorator
    {
        [NodeVariable]
        public float  Time = 1;

        private float mLastTime;
        private int   mTimes;

        public override bool OnEnter()
        {
            mLastTime = UnityEngine.Time.time;
            return Time > 0 && base.OnEnter();
        }

        public override ENST OnExecute()
        {
            FirstChild.OnTick();
            switch (FirstChild.State)
            {
                case ENST.SUCCESS:
                case ENST.FAILURE:
                    FirstChild.OnReset();
                    break;
            }
            mLastTime = UnityEngine.Time.time - StartupTime;
            if (mLastTime >= Time)
            {
                FirstChild.OnExit(ENST.SUCCESS);
                return ENST.SUCCESS;
            }
            else
            {
                return ENST.RUNNING;
            }
        }

#if UNITY_EDITOR
        public override void DrawNodeWindowContents()
        {
            GUILayout.Label(string.Format("<color=#ffffff>持续时间：{0}s</color>", this.Time));
            if (Running)
            {
                GUILayout.Label(string.Format("<color=#ff64cb>运行时间：{0}s</color>", mLastTime > this.Time ? this.Time : mLastTime));
            }
        }
#endif
    }
}