using UnityEngine;
using System.Collections;
using BVT;
using System.Collections.Generic;

namespace BVT
{
    public interface INode
    {
        void OnCreate();
        bool OnEnter();
        void OnTick();
        void OnReset();
        void OnExit(ENST state);
    }
}

