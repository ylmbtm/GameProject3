using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "装饰节点", Label = "失败节点")]
    [NodeDesc("无论子节点返回成功或失败，均返回失败")]
    [NodeName("05CBF2", "ReturnFailure")]
    public class BTReturnFailure : BTDecorator
    {
        public override ENST OnExecute()
        {
            FirstChild.OnTick();
            switch (FirstChild.State)
            {
                case ENST.SUCCESS:
                case ENST.FAILURE:
                    return ENST.FAILURE;
                default:
                    return FirstChild.State;
            }
        }
    }
}
