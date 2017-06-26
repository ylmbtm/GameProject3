using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    [AttributeUsage(AttributeTargets.Class)]
    public class NodeNameAttribute : Attribute
    {
        public string Name   { get; private set; }

        public NodeNameAttribute(string format, string name)
        {
            this.Name = string.Format("<color=#{0}><b>{1}</b></color>", format, name);
        }
    }
}