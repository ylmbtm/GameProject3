using UnityEngine;
using System.Collections;
using BVT;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "动作节点", Label = "BTWait")]
    public class BTWait : BTTask
    {
        [NodeVariable]
        public float WaitTime      = 1;
        [NodeVariable]
        public bool  RandomWait    = false;
        [NodeVariable]
        public float RandomWaitMin = 1;
        [NodeVariable]
        public float RandomWaitMax = 2;

        private float mWaitTime;
        private float mLastTime;

        public override bool OnEnter()
        {
            base.OnEnter();
            this.mWaitTime = RandomWait ? UnityEngine.Random.Range(RandomWaitMin, RandomWaitMax) : WaitTime;
            this.mLastTime = 0;
            return true;
        }

        public override ENST OnExecute()
        {
            mLastTime = Time.time - StartupTime;
            return mLastTime >= mWaitTime ? ENST.SUCCESS : ENST.RUNNING;
        }

        public override bool AutoDrawNodeInspector
        {
            get { return false; }
        }

#if UNITY_EDITOR
        public override void DrawNodeWindowContents()
        {
            if (Running)
            {
                GUILayout.Label(string.Format("<color=#ffffff>等待时间：{0}s</color>", this.mWaitTime));
                GUILayout.Label(string.Format("<color=#ff64cb>运行时间：{0}s</color>", this.mLastTime > this.mWaitTime ? this.mWaitTime : this.mLastTime));
            }
            else
            {
                if (RandomWait)
                {
                    GUILayout.Label(string.Format("<color=#ffffff>等待时间：{0}s~{1}s</color>", this.RandomWaitMin, this.RandomWaitMax));
                }
                else
                {
                    GUILayout.Label(string.Format("<color=#ffffff>等待时间：{0}s</color>", this.WaitTime));
                }
            }
        }

        public override void DrawNodeInspectorGUI()
        {
            base.DrawNodeInspectorGUI();
            this.RandomWait = UnityEditor.EditorGUILayout.Toggle("RandomWait", RandomWait);
            if (!RandomWait)
            {
                float f0 = UnityEditor.EditorGUILayout.FloatField("WaitTime", WaitTime);
                WaitTime = f0 > 0 ? f0 : 0.01f;
            }
            else
            {
                float f1 = UnityEditor.EditorGUILayout.FloatField("RandomWaitMin", RandomWaitMin);
                float f2 = UnityEditor.EditorGUILayout.FloatField("RandomWaitMax", RandomWaitMax);
                RandomWaitMin = f1 <= 0 ? 0 : f1;
                RandomWaitMax = f2 > f1 ? f2 : f1 + 0.001f;
            }
        }
#endif
    }
}