using BVT;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BVT
{
    [NodeAttribute(Type = "组合节点", Label = "并行-队列节点")]
    [NodeDesc("一False则返回False，全True才返回True")]
    [NodeName("ff64cb", "ParallelSequence")]
    public class BTParallelSequence : BTComposite
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
                switch (child.State)
                {
                    case ENST.SUCCESS:
                        mFinishChildIndexList.Add(i);
                        continue;
                    case ENST.FAILURE:
                        mFinishChildIndexList.Add(i);
                        return ENST.FAILURE;
                    case ENST.RUNNING:
                        continue;
                }
            }
            return mFinishChildIndexList.Count >= Children.Count ? ENST.SUCCESS : ENST.RUNNING;
        }

        public override void OnReset()
        {
            base.OnReset();
            mFinishChildIndexList.Clear();
        }
    }
}