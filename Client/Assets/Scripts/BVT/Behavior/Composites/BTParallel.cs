using UnityEngine;
using System.Collections;
using BVT;
using System.Collections.Generic;

namespace BVT
{
    [NodeAttribute(Type = "组合节点", Label = "并行节点")]
    [NodeDesc("并行节点会依次执行所有的子节点，无论子节点返回“成功”或“失败”都会继续运行后续节点")]
    [NodeName("ff64cb", "Parallel")]
    public class BTParallel : BTComposite
    {
        private List<int> mFinishChildIndexList = new List<int>();

        public override ENST OnExecute()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                if (mFinishChildIndexList.Contains(i))
                {
                    continue;
                }
                Node child = Children[i];
                child.OnTick();
                if (child.State != ENST.RUNNING)
                {
                    mFinishChildIndexList.Add(i);
                }
            }
            return mFinishChildIndexList.Count < Children.Count ? ENST.RUNNING : ENST.SUCCESS;
        }

        public override void OnReset()
        {
            base.OnReset();
            mFinishChildIndexList.Clear();
        }
    }
}
