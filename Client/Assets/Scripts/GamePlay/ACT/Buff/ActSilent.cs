using UnityEngine;
using System.Collections;
using ACT;

namespace ACT
{
    public class ActSilent : ActBuffItem
    {
        public override bool Check()
        {
            return Carryer.IsDivive == false;
        }

        public override void Enter()
        {
            this.Carryer.IsSilent = true;
        }

        public override void Stop()
        {
            this.Carryer.IsSilent = false;
        }

        public override void Exit()
        {
            this.Carryer.IsSilent = false;
        }
    }
}

