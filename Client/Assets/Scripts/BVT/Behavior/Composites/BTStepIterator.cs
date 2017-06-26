using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "组合节点", Label = "迭代节点")]
    [NodeName("ff64cb", "StepIterator")]
    public class BTStepIterator : BTComposite
    {
        private int mCurrent;

        public override bool OnEnter()
        {
            base.OnEnter();
            return Children.Count > 0;
        }

        public override ENST OnExecute()
        {
            mCurrent = mCurrent % Children.Count;
            Children[mCurrent].OnTick();
            return Children[mCurrent].State;
        }

        public override void OnReset()
        {
            base.OnReset();
            mCurrent++;
        }
    }
}


