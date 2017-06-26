using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActHit : ActResult
    {
        protected override bool Trigger()
        {
            base.Trigger();
            this.Do();
            return true;
        }

        protected override bool MakeResult(Character cc)
        {
            if (cc == null)
            {
                return false;
            }
            return cc.Command.Get<CommandWound>().Do() == Resp.TYPE_YES;
        }
    }
}

