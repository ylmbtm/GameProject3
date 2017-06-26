using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActTimedDeath : ActBuffItem
    {
        public override bool Check()
        {
            return Carryer.IsDivive == false;
        }

        public override void Exit()
        {
            this.Carryer.KillMe();
        }

        public override void Stop()
        {
            
        }
    }
}

