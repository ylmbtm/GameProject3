using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "条件节点", Label = "CheckBoolean")]
    public class CheckBoolean : BTCondition
    {
        [SerializeField]
        public ShareBool Condition;

        public override void OnCreate()
        {
            this.Condition = NodeFactory.CreateShareObject<ShareBool>(Tree.Blackboard);
        }

        public override bool Check()
        {
            return Condition != null ? (bool)Condition.GetValue() : false;
        }

#if UNITY_EDITOR
        public override void DrawNodeInspectorGUI()
        {
            base.DrawNodeInspectorGUI();
            this.Condition.DrawGUI("Condition");
        }

        public override void DrawNodeWindowContents()
        {
            if (!string.IsNullOrEmpty(Condition.key))
            {
                GUILayout.Label(string.Format("\"<color=#FFA400>{0}</color>\"={1}", Condition.key, Condition.V));
            }
            else
            {
                base.DrawNodeWindowContents();
            }
        }
#endif
    }
}