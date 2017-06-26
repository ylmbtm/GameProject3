using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckHomeDist")]
    public class CheckHomeDist : AICondition
    {
        [SerializeField]
        public ShareFloat Distance;

        public override void OnCreate()
        {
            this.Distance = NodeFactory.CreateShareObject<ShareFloat>(Tree.Blackboard);
        }

        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner == null)
            {
                return false;
            }
            Vector3 srcPos = aiOwner.Pos;
            Vector3 tarPos = aiOwner.BornData.Pos;
            srcPos.y = 0;
            tarPos.y = 0;
            return Vector3.Distance(srcPos, tarPos) < Distance.V;
        }

#if UNITY_EDITOR
        public override void DrawNodeWindowContents()
        {
            if (!Invert)
            {
                string s = string.Format("当与出生点的距离<{0}时", Distance.ToEncode());
                GUILayout.Label(s);
            }
            else
            {
                string s = string.Format("当与出生点的距离>{0}时", Distance.ToEncode());
                GUILayout.Label(s);
            }
        }

        public override void DrawNodeInspectorGUI()
        {
            base.DrawNodeInspectorGUI();
            BTHelper.DrawCoolTitle("ShareObjects");
            this.Distance.DrawGUI("Distance");
        }
#endif
    }
}

