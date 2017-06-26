using UnityEngine;
using System.Collections;
using BVT;

namespace BVT
{
    public class BTComposite : Node
    {
        public override int  MaxChildCount { get { return -1;   } }
        public override bool CanAsFirst    { get { return true; } }
    }
}

