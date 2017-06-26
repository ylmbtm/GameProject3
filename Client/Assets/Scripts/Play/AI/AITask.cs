using UnityEngine;
using System.Collections;

namespace BVT.AI
{
    public class AITask : BTTask
    {
        public Character AIOwner
        {
            get { return AIHelper.GetAIInterface(Agent); }
        }
    }
}

