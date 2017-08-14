using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActStealth : ActBuffItem
    {
        public override void Enter()
        {
            this.Carryer.Command.Get<CommandStealthBegin>().Do();
        }

        public override void Stop()
        {
            this.Carryer.Command.Get<CommandStealthLeave>().Do();
        }

        public override void Exit()
        {
            this.Carryer.Command.Get<CommandStealthLeave>().Do();
        }
    }
}

