using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace ACT
{
    public class ActRandomMany : ActItem
    {
        [SerializeField]
        public int            RandomNum  = 1; //随机选取的数量

        public List<ActItem>  RandomList
        {
            get;
            private set;
        }

        public ActRandomMany()
        {
            EventType = EActEventType.Instant;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            this.RandomList = GTTools.RandomListFromBigList(Children, RandomNum);
            return true;
        }

        protected override void ExecuteChildren()
        {
            this.LoopChildren(RandomList);
        }
    }
}

