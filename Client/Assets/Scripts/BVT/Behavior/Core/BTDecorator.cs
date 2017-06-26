using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BVT;

namespace BVT
{
    public class BTDecorator : Node
    {
        public override int  MaxChildCount { get { return 1; } }
        public override bool CanAsFirst    { get { return true; } }

        public override bool OnEnter()
        {
            base.OnEnter();
            return FirstChild != null;
        }
    }
}
