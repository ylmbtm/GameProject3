using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    [AttributeUsage(AttributeTargets.Class)]
    public class NodeDescAttribute : Attribute
    {
        public string Desc { get; private set; }

        public NodeDescAttribute(string desc)
        {
            this.Desc = desc;
        }
    }
}