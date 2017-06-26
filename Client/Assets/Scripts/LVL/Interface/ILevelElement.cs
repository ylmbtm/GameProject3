using UnityEngine;
using System.Collections;

namespace LVL
{
    public interface ILevelElement : IObj
    {
        void Build();
        void Import(DCFG cfg);
        DCFG Export();
    }
}

