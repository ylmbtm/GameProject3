using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActStun : ActBuffItem
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
            if (Carryer.FSM == FSMState.FSM_STUN)
            {
                return true;
            }
            if (Carryer.IsFSMLayer3())
            {
                return false;
            }
            return true;
        }

        public override void Enter()
        {
            this.Carryer.Command.Get<CommandStun>().Update(Duration).Do();
        }

        public override void Refresh()
        {
            this.Carryer.Command.Get<CommandStun>().Update(Duration).Do();
        }

        public override void Stop()
        {
            this.Carryer.DoEmpty();
        }
    }
}

