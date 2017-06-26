using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    [NodeAttribute(Type = "动作节点", Label = "BTRun")]
    public class BTRun : BTTask
    {
        public override ENST OnExecute()
        {
            return ENST.RUNNING;
        }
    }
}

