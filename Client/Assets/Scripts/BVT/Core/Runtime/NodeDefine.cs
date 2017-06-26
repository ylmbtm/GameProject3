using UnityEngine;
using System.Collections;

namespace BVT
{
    public enum ENST
    {
        INITIAL=0,
        FAILURE=1,
        SUCCESS=2,
        RUNNING=3,
    }

    public enum ETreeType
    {
        TYPE_SKILL,
        TYPE_AI,
    }

    public enum ENodeType
    {
        TYPE_ACTION,
        TYPE_COMPOSITE,
        TYPE_CONDITION,
        TYPE_DECORATOR,
    }

    public enum ENodeConnMode
    {
        Bezier,
        Manhattan,
        Straight
    }
}