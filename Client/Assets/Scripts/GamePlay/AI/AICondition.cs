using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    public class AICondition : BTConditionComposite
    {
        public Character AIOwner
        {
            get { return AIHelper.GetAIInterface(Agent); }
        }
    }
}


