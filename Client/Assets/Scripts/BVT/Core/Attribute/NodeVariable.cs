using UnityEngine;
using System.Collections;
using System;

namespace BVT
{
    [AttributeUsage(AttributeTargets.Property | AttributeTargets.Field)]
    public class NodeVariable : Attribute
    {
        public string Tip { get; private set; }

        public NodeVariable()
        {

        }

        public NodeVariable(string tip)
        {
            this.Tip = tip;
        }
    }
}