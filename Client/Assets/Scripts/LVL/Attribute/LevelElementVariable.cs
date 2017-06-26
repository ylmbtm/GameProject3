using UnityEngine;
using System.Collections;
using System;
using CFG;

namespace LVL
{
    [AttributeUsage(AttributeTargets.Class)]
    public class LevelElementVariable : Attribute
    {
        public Type CfgType { get; set; }
    }
}
