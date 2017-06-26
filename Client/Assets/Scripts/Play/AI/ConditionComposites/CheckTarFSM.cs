using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckTarFSM")]
    public class CheckTarFSM : AICondition
    {
        [SerializeField]
        public FSMState ApplyFSM;

        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            if (aiOwner == null)
                return false;
            if (aiOwner.Target == null)
                return false;
            return aiOwner.Target.FSM == ApplyFSM;
        }

#if UNITY_EDITOR
        public override void DrawNodeWindowContents()
        {
            if (!Invert)
            {
                string s = string.Format("若目标现在是{0}状态", ApplyFSM);
                GUILayout.Label(s);
            }
            else
            {
                string s = string.Format("若目标现在不是{0}状态", ApplyFSM);
                GUILayout.Label(s);
            }
        }

        public override void DrawNodeInspectorGUI()
        {
            base.DrawNodeInspectorGUI();
            this.ApplyFSM = (FSMState)UnityEditor.EditorGUILayout.EnumPopup("ApplyFSM", (System.Enum)ApplyFSM);
        }
#endif
    }
}

