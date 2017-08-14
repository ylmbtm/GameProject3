using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckTarAIState")]
    public class CheckTarAIState : AICondition
    {
        [SerializeField]
        public EAIState ApplyAIState;

        public override string Name
        {
            get { return !Invert ? "目标AI Is " + ApplyAIState.ToString() : string.Format("目标AI Not {0}", ApplyAIState); }
        }

        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            return aiOwner == null ? false : aiOwner.AIState == ApplyAIState;
        }

#if UNITY_EDITOR
        public override void DrawNodeInspectorGUI()
        {
            base.DrawNodeInspectorGUI();
            this.ApplyAIState = (EAIState)UnityEditor.EditorGUILayout.EnumPopup("ApplyAIState", ApplyAIState);
        }
#endif
    }
}

