using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

namespace NLE
{
    public class NContainer<T> : NItem where T : NItem
    {
        private List<T> mChildren = new List<T>();
        private bool    mIsInited = false;

        public List<T>    Children
        {
            get
            {
#if UNITY_EDITOR
                mChildren.Clear();
                GetComponentsInChildren<T>();
#else
                if (mIsInited)
                {
                    mChildren.Clear();
                    GetComponentsInChildren<T>(mChildren);
                    mIsInited = true;
                }              
#endif
                return mChildren;
            }
        }

        public NSequencer Sequencer
        {
            get
            {
                NSequencer ns = transform.GetComponentInParent<NSequencer>();
                return ns;
            }
        }
 
        public override void Init()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].Init();
            }
            base.Init();
        }

        public override void Begin()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].Begin();
            }
            base.Begin();
        }

        public override void End()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].End();
            }
            base.End();
        }

        public override void Loop(float time, float delta)
        {
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].Loop(time, delta);
            }
            base.Loop(time, delta);
        }

        public override void Pause()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].Pause();
            }
            base.Pause();
        }

        public override void Resume()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].Resume();
            }
            base.Resume();
        }

        public override void Reverse()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].Reverse();
            }
            base.Reverse();
        }

        public override void Sample(float time)
        {
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].Sample(time);
            }
            base.Sample(time);
        }

        public override void Stop()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].Stop();
            }
            base.Stop();
        }

        public override void Trigger()
        {
            for (int i = 0; i < Children.Count; i++)
            {
                Children[i].Trigger();
            }
            base.Trigger();
        }
    }
}
