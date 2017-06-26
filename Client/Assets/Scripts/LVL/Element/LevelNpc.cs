using UnityEngine;
using System.Collections;
using CFG;
using System.Collections.Generic;

namespace LVL
{
    [LevelElementVariable(CfgType = typeof(MapNpc))]
    public class LevelNpc : LevelElement
    {
        [LevelVariable]
        public List<string> Talks = new List<string>();
    }
}

