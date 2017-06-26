using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "动作节点", Label = "BTWaitFrames")]
    public class BTWaitFrames : BTTask
    {
        [NodeVariable]
        public  int  WaitFrames    = 1;
        [NodeVariable]
        public  bool RandomWait    = false;
        [NodeVariable]
        public  int  RandomWaitMin = 1;
        [NodeVariable]
        public  int  RandomWaitMax = 2;

        private int mDrFrames   = 0;
        private int mStFrames = 0;

        public override bool OnEnter()
        {
            base.OnEnter();
            this.mDrFrames = RandomWait ? UnityEngine.Random.Range(RandomWaitMin, RandomWaitMax + 1) : WaitFrames;
            this.mStFrames = Time.frameCount;
            return true;
        }

        public override ENST OnExecute()
        {
            if (Time.frameCount - mStFrames >= mDrFrames)
            {
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
            if (Running)
            {
                int lastFrames = Time.frameCount - mStFrames;
                GUILayout.Label(string.Format("<color=#ffffff>等待帧数：{0}</color>", mDrFrames));
                GUILayout.Label(string.Format("<color=#ff64cb>运行帧数：{0}</color>", lastFrames > this.mDrFrames ? this.mDrFrames : lastFrames));
            }
            else
            {
                if (RandomWait)
                {
                    GUILayout.Label(string.Format("<color=#ffffff>等待帧数：{0}s~{1}</color>", this.RandomWaitMin, this.RandomWaitMax));
                }
                else
                {
                    GUILayout.Label(string.Format("<color=#ffffff>等待帧数：{0}</color>", this.WaitFrames));
                }
            }
        }

        public override void DrawNodeInspectorGUI()
        {
            base.DrawNodeInspectorGUI();
            this.RandomWait = UnityEditor.EditorGUILayout.Toggle("RandomWait", RandomWait);
            if (!RandomWait)
            {
                int f0 = UnityEditor.EditorGUILayout.IntField("WaitFrames", WaitFrames);
                WaitFrames = f0;
            }
            else
            {
                int f1 = UnityEditor.EditorGUILayout.IntField("RandomWaitMin", RandomWaitMin);
                int f2 = UnityEditor.EditorGUILayout.IntField("RandomWaitMax", RandomWaitMax);
                RandomWaitMin = f1 <= 0 ? 0 : f1;
                RandomWaitMax = f2 > f1 ? f2 : f1 + 1;
            }
        }
#endif
    }
}
