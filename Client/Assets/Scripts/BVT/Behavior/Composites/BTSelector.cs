using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "组合节点", Label = "选择节点")]
    [NodeDesc("选择节点，顺序执行子节点，只要碰到一个子节点返回TRUE，则返回TRUE；否则返回FALSE")]
    [NodeName("b3ff7f", "Selector")]
    public class BTSelector : BTComposite
    {
        private int mActiveChildIndex = 0;

        public override ENST OnExecute()
        {
            for (; mActiveChildIndex < Children.Count; mActiveChildIndex++)
            {
                Node child = Children[mActiveChildIndex];
                child.OnTick();
                switch (child.State)
                {
                    case ENST.RUNNING:
                        {
                            return ENST.RUNNING;
                        }
                    case ENST.SUCCESS:
                        {
                            return ENST.SUCCESS;
                        }
                    case ENST.FAILURE:
                        {
                            continue;                          
                        }
                }
            }
            return ENST.FAILURE;
        }

        public override void OnReset()
        {
            base.OnReset();
            mActiveChildIndex = 0;
        }
    }
}
