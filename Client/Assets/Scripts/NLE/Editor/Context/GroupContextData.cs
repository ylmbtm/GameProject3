using UnityEngine;
using System.Collections;
using System;

namespace NLE
{
    public class GroupContextData
    {
        public Type   Type;
        public string Label = string.Empty;

        public GroupContextData(Type type, string label)
        {
            this.Type  = type;
            this.Label = label;
        }
    }
}
