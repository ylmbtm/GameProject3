using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "装饰节点", Label = "BTUntilFailure")]
    [NodeDesc("直到子节点返回失败，才返回失败")]
    [NodeName("05CBF2", "UntilFailure")]
    public class BTUntilFailure : BTDecorator
    {
        public override ENST OnExecute()
        {
            FirstChild.OnTick();
            switch (FirstChild.State)
            {
                case ENST.FAILURE:
                    return ENST.FAILURE;
                case ENST.SUCCESS:
                    FirstChild.OnReset();
                    return ENST.RUNNING;
                default:
                    return ENST.RUNNING;
            }
        }
    }
}

