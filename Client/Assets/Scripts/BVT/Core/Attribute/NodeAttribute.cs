using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    [AttributeUsage(AttributeTargets.Class)]
    public class NodeAttribute : Attribute
    {
        public string    Type  = string.Empty;
        public string    Label = string.Empty;
        public string    Group = string.Empty;
        public Type      NodeType { get; set; }
    }
}

