using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    public class BTConditionComposite : Node
    {
        [NodeVariable]
        public bool Invert = false;

        public override int  MaxChildCount { get { return 2; } }
        public override bool CanAsFirst { get { return false; } }

        private Node mNode1;
        private Node mNode2;
        private Node mCurrentExecute;

        public sealed override bool OnEnter()
        {
            base.OnEnter();
            if (Children.Count > 0)
            {
                mNode1 = Children[0];
            }
            if (Children.Count > 1)
            {
                mNode2 = Children[1];
            }
            if (Invert == false)
            {
                mCurrentExecute = Check() ? mNode1 : mNode2;
            }
            else
            {
                mCurrentExecute = Check() ? mNode2 : mNode1;
            }
            return true;
        }

        public sealed override ENST OnExecute()
        {
            if (mCurrentExecute == null)
            {
                if(Invert == false)
                {
                    return Check() ? ENST.SUCCESS : ENST.FAILURE;
                }
                else
                {
                    return Check() ? ENST.FAILURE : ENST.SUCCESS;
                }
            }
            else
            {
                mCurrentExecute.OnTick();
                return mCurrentExecute.State;
            }           
        }

        public virtual bool Check()
        {
            return true;
        }
    }
}

