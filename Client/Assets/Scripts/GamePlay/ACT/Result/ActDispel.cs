using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActDispel : ActResult
    {
        [SerializeField]
        public EBuffType DispelType   = EBuffType.Buff;
        [SerializeField]
        public int       DispelMaxNum = 1;

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
            cc.Skill.DelBuffByType(DispelType, DispelMaxNum);
            return true;
        }
    }
}