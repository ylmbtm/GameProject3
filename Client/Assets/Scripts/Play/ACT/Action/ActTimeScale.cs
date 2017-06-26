using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActTimeScale : ActItem
    {
        [SerializeField]
        public int            CurveID;

        public AnimationCurve Curve
        {
            get; private set;
        }

        public ActTimeScale()
        {
            this.EventType = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            DCurve d = ReadCfgCurve.GetDataById(CurveID);
            if (d != null)
            {
                this.Curve = ECurve.Get(d.Path);
            }
            return this.Curve != null;
        }

        protected override void Execute()
        {
            base.Execute();
            float v = Curve.Evaluate(PastTime);
            Time.timeScale = v;

        }

        protected override void End()
        {
            Time.timeScale = 1;
            base.End();
        }

        public override void Clear()
        {
            Time.timeScale = 1;
            base.Clear();
        }
    }
}
