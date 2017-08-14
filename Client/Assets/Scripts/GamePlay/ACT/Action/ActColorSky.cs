using UnityEngine;
using System.Collections;

namespace ACT
{
    public class ActColorSky : ActItem
    {
        [SerializeField]
        public int R = 255;
        [SerializeField]
        public int G = 255;
        [SerializeField]
        public int B = 255;
        [SerializeField]
        public int A = 255;

        public Color TempColor
        {
            get; private set;
        }

        public ActColorSky()
        {
            this.EventType = EActEventType.Subtain;
        }

        protected override bool Trigger()
        {
            base.Trigger();
            this.TempColor = GTCameraManager.Instance.MainCamera.backgroundColor;
            GTCameraManager.Instance.MainCamera.backgroundColor = new Color(R, G, B, A);
            return true;
        }

        protected override void End()
        {
            base.End();
            GTCameraManager.Instance.MainCamera.backgroundColor = this.TempColor;
        }
    }
}

