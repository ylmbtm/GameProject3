using UnityEngine;
using System.Collections;
using System;
using NLE;
using System.Collections.Generic;

namespace NLE
{
    public class NSequencer : MonoBehaviour, INSequencer
    {
        [SerializeField]
        public float Duration    = 30;
        [SerializeField]
        public float PlaySpeed   = 1;
        [SerializeField]
        public bool  IsSkippable = true;
        [SerializeField]
        public bool  IsLooping   = false;

        public List<NGroup>      Groups
        {
            get
            {
#if UNITY_EDITOR
                mGroups.Clear();
                GetComponentsInChildren<NGroup>(mGroups);
#else
                if (mIsInitialized)
                {
                    mGroups.Clear();
                    GetComponentsInChildren<NGroup>(mGroups);
                    mIsInitialized = true;
                }
#endif
                return mGroups;
            }
        }
        public CutsceneState     State       { get; set; }
        public float             RunningTime { get; set; }   
        public NSequencerHandler OnFinish    { get; set; }
        public NSequencerHandler OnPaused    { get; set; }

        private bool             mIsInitialized = false;
        private List<NGroup>     mGroups        = new List<NGroup>();

        public void Init()
        {
            this.Groups.ForEach((a) => { a.Init(); });
            this.mIsInitialized = true;
        }

        public void Play()
        {
            switch(State)
            {
                case CutsceneState.Inactive:
                    StartCoroutine(FreshPlay());
                    break;
                case CutsceneState.Paused:
                    State = CutsceneState.Playing;
                    StartCoroutine(UpdateCoroutine());
                    break;
            }
        }

        private IEnumerator UpdateCoroutine()
        {
            bool firstFrame = true;
            while(State==CutsceneState.Playing)
            {
                if(firstFrame)
                {
                    UpdateCutscene(0);
                    firstFrame = false;
                }
                else
                {
                    UpdateCutscene(Time.deltaTime);
                }
            }
            yield return null;
        }

        private IEnumerator PreparePlay()
        {
            if (!mIsInitialized)
            {
                Init();
            }
            yield return null;
        }

        private IEnumerator FreshPlay()
        {
            yield return StartCoroutine(PreparePlay());
            yield return null;
            State = CutsceneState.Playing;
            StartCoroutine(UpdateCoroutine());
        }

        public void Pause()
        {
            if (State == CutsceneState.Playing)
            {
                StopCoroutine(UpdateCoroutine());
            }
            if (State == CutsceneState.PreviewPlaying ||
                State == CutsceneState.Playing        ||
                State == CutsceneState.Scrubbing)
            {
                Groups.ForEach((a) => { a.Pause(); });
            }
            this.State = CutsceneState.Paused;
            if (OnPaused != null)
            {
                OnPaused.Invoke(this);
            }
        }

        public void Skip()
        {
            if(IsSkippable)
            {
                SetRunningTime(this.Duration);
                State = CutsceneState.Inactive;
                Stop();
            }
        }

        public void ExitPreviewMode()
        {
            Stop();
        }

        public void ScrubToTime(float newTime)
        {
            float deltaTime = Mathf.Clamp(newTime, 0, Duration) - this.RunningTime;
            State = CutsceneState.Scrubbing;
            if (deltaTime == 0)
            {
                Pause();
            }
            else
            {
                UpdateCutscene(deltaTime);
            }
        }

        public void UpdateCutscene(float deltaTime)
        {
            this.RunningTime += deltaTime * PlaySpeed;
            this.Groups.ForEach((a) => { a.Loop(RunningTime, deltaTime * PlaySpeed); });
            if (State != CutsceneState.Scrubbing)
            {
                if (RunningTime >= Duration || RunningTime < 0)
                {
                    Stop();
                }
            }
        }

        public void Refresh()
        {
            if (State != CutsceneState.Inactive)
            {
                float tempTime = RunningTime;
                Stop();
                EnterPreviewMode();
                SetRunningTime(tempTime);
            }
        }

        public void PreviewPlay()
        {
            if (State == CutsceneState.Inactive)
            {
                EnterPreviewMode();
            }
            switch(State)
            {
                case CutsceneState.Inactive:
                    EnterPreviewMode();
                    break;
                case CutsceneState.Paused:
                    Resume();
                    break;
            }
            if(Application.isPlaying)
            {
                State = CutsceneState.Playing;
            }
            else
            {
                State = CutsceneState.PreviewPlaying;
            }
        }

        public void SetRunningTime(float time)
        {
            this.Groups.ForEach((a) => { a.Sample(time); });
            this.RunningTime = time;
        }

        public void EnterPreviewMode()
        {
            if (State == CutsceneState.Inactive)
            {
                Init();
                SetRunningTime(RunningTime);
                State = CutsceneState.Paused;
            }
        }

        public void Stop()
        {
            this.RunningTime = 0;
            this.Groups.ForEach((a) => { a.Stop(); });
            if (State == CutsceneState.Playing)
            {
                StopCoroutine(UpdateCoroutine());
                if (State == CutsceneState.Playing && IsLooping)
                {
                    State = CutsceneState.Inactive;
                    Play();
                }
                else
                {
                    State = CutsceneState.Inactive;
                }
            }
            else
            {
                State = CutsceneState.Inactive;
            }
            if (State == CutsceneState.Inactive)
            {
                if (OnFinish != null)
                {
                    OnFinish(this);
                }
            }
        }

        void Resume()
        {
            this.Groups.ForEach((a) => { a.Resume(); });
        }

        void OnDestroy()
        {
            if (!Application.isPlaying)
            {
                Stop();
            }
        }
    }
}
