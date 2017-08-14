using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckHostDist")]
    public class CheckHostDist : AICondition
    {
        [NodeVariable]
        public float Distance = 10;

        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner == null)
            {
                return false;
            }
            if (aiOwner.Host == null)
            {
                return false;
            }
            Vector3 srcPos = aiOwner.Pos;
            Vector3 tarPos = aiOwner.Host.Pos;
            srcPos.y = 0;
            tarPos.y = 0;
            return Vector3.Distance(srcPos, tarPos) < Distance;
        }

#if UNITY_EDITOR
        public override void DrawNodeWindowContents()
        {
            if (!Invert)
            {
                string s = string.Format("当与主人的距离<{0}时", Distance);
                GUILayout.Label(s);
            }
            else
            {
                string s = string.Format("当与主人的距离>{0}时", Distance);
                GUILayout.Label(s);
            }
        }
#endif

    }
}

