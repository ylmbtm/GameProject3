using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    public class BTCondition : BTTask
    {
        [NodeVariable]
        public bool Invert = false;

        public sealed override bool OnEnter()
        {
            base.OnEnter();
            return Invert ? !Check() : Check();
        }

        public virtual         bool Check()
        {
            return true;
        }

        public override        ENST OnExecute()
        {
            return ENST.SUCCESS;
        }
    }
}
