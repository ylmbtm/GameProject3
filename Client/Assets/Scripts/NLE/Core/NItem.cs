using UnityEngine;
using System.Collections;
using NLE;
using System;

namespace NLE
{
    public class NItem : MonoBehaviour, INItem
    {
        [SerializeField]
        public float FireTime;
        [SerializeField]
        public float Duration;

        public float CurrTime
        {
            get;
            private set;
        }

        public float PrevTime
        {
            get;
            private set;
        }

        public float EndTime
        {
            get { return FireTime + Duration; }
        }

        public virtual void Init()
        {

        }

        public virtual void Begin()
        {
            this.CurrTime = 0;
            this.PrevTime = 0;
        }

        public virtual void Loop(float time, float delta)
        {
            this.PrevTime = this.CurrTime;
            this.CurrTime = time;
            if (FireTime >= this.PrevTime && FireTime <= this.CurrTime)
            {
                Trigger();
            }
            if (FireTime <= this.PrevTime && FireTime >= this.CurrTime)
            {
                Reverse();
            }
            if (this.CurrTime >= this.EndTime)
            {
                End();
            }
        }

        public virtual void Sample(float time)
        {
            this.PrevTime = this.CurrTime;
            this.CurrTime = time;
            if (FireTime >= this.PrevTime && FireTime <= this.CurrTime)
            {
                Trigger();
            }
            if (FireTime <= this.PrevTime && FireTime >= this.CurrTime)
            {
                Reverse();
            }
        }

        public virtual void Stop()
        {
            this.End();
        }

        public virtual void End()
        {
            this.CurrTime = this.Duration;
        }

        public virtual void Pause()
        {

        }

        public virtual void Resume()
        {

        }

        public virtual void Trigger()
        {

        }

        public virtual void Reverse()
        {

        }
    }
}

