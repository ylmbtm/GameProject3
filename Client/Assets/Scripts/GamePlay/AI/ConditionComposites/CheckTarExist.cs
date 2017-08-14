using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    [NodeAttribute(Type = "AI/条件组合节点", Label = "CheckTarExist")]
    [NodeName("ff64cb", "<size=12>CheckTarExist</size>")]
    public class CheckTarExist : AICondition
    {
        public override bool Check()
        {
            Character aiOwner = this.AIOwner;
            return aiOwner == null ? false : aiOwner.Target != null;
        }

#if UNITY_EDITOR
        public override string DrawConnectionForChild(Node node, int index)
        {
            if (index == 0)
            {
                return Invert == false ? "存在目标时" : "不存在目标时";
            }
            else
            {
                return Invert == false ? "不存在目标时" : "存在目标时";
            }
        }
#endif
    }
}

