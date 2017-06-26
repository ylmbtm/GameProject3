using UnityEngine;
using System.Collections;
using BVT;
using System.Collections.Generic;

namespace BVT
{
    [NodeAttribute(Type = "组合节点", Label = "串行节点")]
    [NodeDesc("串行节点，顺序执行子节点，只要碰到一个子节点返回FALSE，则返回FALSE；否则返回TRUE")]
    [NodeName("bf7fff", " Sequence")]
    public class BTSequence : BTComposite
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
                            continue;
                        }
                    case ENST.FAILURE:
                        {
  
                            return ENST.FAILURE;
                        }
                }
            }
            return ENST.SUCCESS;
        }

        public override void OnReset()
        {
            base.OnReset();
            mActiveChildIndex = 0;
        }
    }
}
