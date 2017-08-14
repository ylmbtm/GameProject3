using UnityEngine;
using System.Collections;
using System;

namespace MAP
{
    public class FTCondition : MonoBehaviour, IFTCondition
    {
        public virtual bool Check()
        {
            return true;
        }
    }
}