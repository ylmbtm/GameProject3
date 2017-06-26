using BVT;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BVT
{
    [NodeAttribute(Type = "组合节点", Label = "并行-选择节点")]
    [NodeDesc("一True则返回True，全False才返回False")]
    [NodeName("ff64cb", "ParallelSelector")]
    public class BTParallelSelector : BTComposite
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
                switch(child.State)
                {
                    case ENST.SUCCESS:
                        mFinishChildIndexList.Add(i);
                        return ENST.SUCCESS;
                    case ENST.FAILURE:
                        mFinishChildIndexList.Add(i);
                        continue;
                    case ENST.RUNNING:
                        continue;
                }
            }
            return mFinishChildIndexList.Count >= Children.Count ? ENST.FAILURE : ENST.RUNNING;
        }

        public override void OnReset()
        {
            base.OnReset();
            mFinishChildIndexList.Clear();
        }
    }
}
