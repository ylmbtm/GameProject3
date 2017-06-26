using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActBeatBack : ActResult
    {
        [SerializeField]
        public float MaxDis = 8;

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
            return cc.Command.Get<CommandBeatBack>().Update(MaxDis).Do() == Resp.TYPE_YES;
        }
    }
}

