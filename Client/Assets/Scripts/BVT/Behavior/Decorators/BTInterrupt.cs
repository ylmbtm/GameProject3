using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "装饰节点", Label = "中断节点")]
    [NodeDesc("如果条件满足，则返回失败；否则，则一直运行直至子节点返回成功或者返回失败")]
    [NodeName("05CBF2", "Interrupt")]
    public class BTInterrupt : BTDecorator
    {
        [SerializeField]
        public ShareBool Condition;

        public override void OnCreate()
        {
            this.Condition = NodeFactory.CreateShareObject<ShareBool>(this.Tree.Blackboard);
        }

        public override bool OnEnter()
        {
            base.OnEnter();
            return FirstChild != null;
        }

        public override ENST OnExecute()
        {
            if (Condition.V == false)
            {
                FirstChild.OnTick();
                return FirstChild.State;
            }
            else
            {
                FirstChild.OnExit(ENST.FAILURE);
                return ENST.FAILURE;
            }
        }

#if UNITY_EDITOR
        public override void DrawNodeWindowContents()
        {
            base.DrawNodeWindowContents();
            GUILayout.Label(string.Format("IF {0}", Condition.key));
        }

        public override void DrawNodeInspectorGUI()
        {
            base.DrawNodeInspectorGUI();
            this.Condition.DrawGUI("Condition");
        }
#endif
    }
}

