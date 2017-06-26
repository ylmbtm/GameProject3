using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActFixBody : ActBuffItem
    {
        public override bool Check()
        {
            if (Carryer.IsDivive)
            {
                return false;
            }
            if (Carryer.IsRide)
            {
                return false;
            }
            return true;
        }

        public override void Enter()
        {
            this.Carryer.Command.Get<CommandFixBodyBegin>().Do();
        }


        public override void Exit()
        {
            this.Carryer.Command.Get<CommandFixBodyLeave>().Do();
        }

        public override void Stop()
        {
            this.Carryer.Command.Get<CommandFixBodyLeave>().Do();
        }
    }
}

