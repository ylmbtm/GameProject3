using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    public class AIHelper
    {
        public static Character GetAIInterface(Transform trans)
        {
            if (trans == null)
            {
                return null;
            }
            CharacterView view = trans.GetComponent<CharacterView>();
            if (view == null)
            {
                return null;
            }
            return view.Owner;
        }

        public static Character GetAIInterface(GameObject go)
        {
            if (go == null)
            {
                return null;
            }
            CharacterView view = go.GetComponent<CharacterView>();
            if (view == null)
            {
                return null;
            }
            return view.Owner;
        }

        public static Character GetAIInterface(ShareTransform shareObj)
        {
            if (shareObj != null)
            {
                return GetAIInterface(shareObj.V);
            }
            else
            {
                return null;
            }
        }

        public static string    GetAIStateName(EAIState aiState)
        {
            switch (aiState)
            {
                case EAIState.IDLE:
                    return "闲置";
                case EAIState.BORN:
                    return "出生";
                case EAIState.PATROL:
                    return "巡逻";
                case EAIState.FOLLOW:
                    return "跟随";
                case EAIState.ESCAPE:
                    return "逃跑";
                case EAIState.CHASE:
                    return "追击";
                case EAIState.BACK:
                    return "回家";
                case EAIState.FIGHT:
                    return "攻击";
                case EAIState.DEAD:
                    return "死亡";
                default:
                    return "未知";
            }
        }
    }
}