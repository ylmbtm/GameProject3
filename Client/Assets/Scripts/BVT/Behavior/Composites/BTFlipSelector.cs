using UnityEngine;
using System.Collections;
using BVT;
using System.Collections.Generic;

namespace BVT
{
    [NodeAttribute(Type = "组合节点", Label = "翻转-选择节点")]
    [NodeName("ff64cb", "FlipSelector")]
    public class BTFlipSelector : BTComposite
    {
        private int        mCurrent;
        private List<int>  mChildExecuteList = new List<int>();

        public override bool OnEnter()
        {
            mChildExecuteList.Clear();
            for (int i = 0; i < Children.Count; i++)
            {
                mChildExecuteList.Add(i);
            }
            return base.OnEnter();
        }

        public override ENST OnExecute()
        {
            for (int i = mCurrent; i <mChildExecuteList.Count; i++)
            {
                Node child = Children[i];
                child.OnTick();
                switch (child.State)
                {
                    case ENST.RUNNING:
                        mCurrent = i;
                        return ENST.RUNNING;
                    case ENST.SUCCESS:
                        OnSendToBack(i);
                        return ENST.SUCCESS;
                }
            }
            return ENST.FAILURE;
        }

        public void          OnSendToBack(int i)
        {
            int index = mChildExecuteList[i];
            mChildExecuteList.RemoveAt(i);
            mChildExecuteList.Add(index);
        }

        public override void OnReset()
        {
            base.OnReset();
            mCurrent = 0;
            mChildExecuteList.Clear();
        }
    }
}

