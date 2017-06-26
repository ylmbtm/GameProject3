using UnityEngine;
using System.Collections;

namespace BVT
{
    public class Port
    {
        public int     PortIndex;
        public Node    Parent;
        public Vector2 Pos;

        public Port(int index, Node parent, Vector2 pos)
        {
            this.PortIndex = index;
            this.Parent    = parent;
            this.Pos       = pos;
        }
    }
}

