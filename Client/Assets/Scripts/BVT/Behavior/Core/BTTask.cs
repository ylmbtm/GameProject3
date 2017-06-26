using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    public class BTTask : Node
    {
        public override int  MaxChildCount { get { return 0; } }

        public override bool CanAsFirst    { get { return false; } }
    }
}
