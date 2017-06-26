using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckFSM")]
    public class CheckFSM : AICondition
    {
        [SerializeField]
        public FSMState        ApplyFSM;

        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            return aiOwner == null ? false : aiOwner.FSM == ApplyFSM;
        }

#if UNITY_EDITOR
        public override void DrawNodeWindowContents()
        {
            if (!Invert)
            {
                string s = string.Format("若现在是{0}状态", ApplyFSM);
                GUILayout.Label(s);
            }
            else
            {
                string s = string.Format("若现在不是{0}状态", ApplyFSM);
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
