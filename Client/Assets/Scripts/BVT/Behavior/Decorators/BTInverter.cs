using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "装饰节点", Label = "逆转节点")]
    [NodeDesc("子节点返回成功，则返回失败；子节点返回失败，则返回成功")]
    [NodeName("05CBF2", "Inverter")]
    public class BTInverter : BTDecorator
    {
        public override ENST OnExecute()
        {
            FirstChild.OnTick();
            switch (FirstChild.State)
            {
                case ENST.SUCCESS:
                    return ENST.FAILURE;
                case ENST.FAILURE:
                    return ENST.SUCCESS;
                default:
                    return FirstChild.State;
            }
        }
    }
}
