using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckHostAIState")]
    public class CheckHostAIState : AICondition
    {
        [SerializeField]
        public EAIState ApplyAIState;

        public override string Name
        {
            get { return !Invert ? "主人AI Is " + ApplyAIState.ToString() : string.Format("主人AI Not {0}", ApplyAIState); }
        }

        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            Character aiHost = aiOwner == null ? null : aiOwner.Host;
            return aiHost == null ? false : aiHost.AIState == ApplyAIState;
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

