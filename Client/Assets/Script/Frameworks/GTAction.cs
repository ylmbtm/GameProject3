using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class GTAction
{
    private Animator                           mAnimator;

    private float                              mPlaySpeed  = 1f;
    private float                              mPlayTimer  = 0f;
    private float                              mCurAnimLength = 0f;
    private Callback                           mCurFinishCallback;
    private string                             mCurAnimName;
    private bool                               mIsStart;
    private bool                               mIsLoop;
    private bool                               mIsFinish;
    private AnimatorStateInfo                  mAnimatorState;


    private Dictionary<string, AnimationClip>  mAnimClips      = new Dictionary<string, AnimationClip>();
    private static HashSet<string>             mNoFadeList     = new HashSet<string> { "idle", "run", "walk" };

    public GTAction(Animator animator)
    {
        mAnimator = animator;
        if (mAnimator == null || mAnimator.runtimeAnimatorController == null)
        {
            return;
        }
        mIsStart = false;
        for (int i = 0; i < mAnimator.runtimeAnimatorController.animationClips.Length; i++)
        {
            AnimationClip clip = mAnimator.runtimeAnimatorController.animationClips[i];
            mAnimClips[clip.name] = clip;
        }
        mActions[mAnimator] = this;
    }

    void Execute()
    {
        if (!mAnimator.enabled)
        {
            return;
        }
        mAnimatorState = mAnimator.GetNextAnimatorStateInfo(0);
        if (!mIsStart)
        {
            if (mAnimatorState.IsName(mCurAnimName))
            {
                mAnimator.SetBool(mCurAnimName, false);
                mIsStart = true;
            }
        }

        if (Time.time - mPlayTimer > mCurAnimLength * mPlaySpeed)
        {
            if (mCurFinishCallback != null)
            {
                mCurFinishCallback();
            }
            mIsFinish = true;
        }
    }

    void Release()
    {
        mCurAnimName = string.Empty;
        mDeleteList.Add(this);
    }

    public void  Play(string animName, Callback onFinish = null, bool isLoop = false, float speed = 1f, float lastTime = 0f)
    {
        if (mCurAnimName == animName || mAnimator == null || !mAnimator.enabled || string.IsNullOrEmpty(animName))
        {
            return;
        }
        if (!string.IsNullOrEmpty(mCurAnimName))
        {
            mAnimator.SetBool(mCurAnimName, false);
        }
        mAnimator.SetBool(animName, true);

        if (!mNoFadeList.Contains(mCurAnimName) && isLoop == false)
        {
            mAnimator.CrossFade(animName, 0.3f);
        }

        mPlayTimer = Time.time;
        mPlaySpeed = speed;
        mCurAnimName = animName;
        mCurFinishCallback = onFinish;
        mCurAnimName = animName;
        mIsStart = false;
        mIsFinish = false;
        mIsLoop = isLoop;
        mCurAnimLength = lastTime > 0 ? lastTime : GetLength(mCurAnimName);
    }

    public void  Play(List<string> animQueue, Callback onFinish, float speed = 1f)
    {
        GTCoroutinueManager.Instance.StartCoroutine(PlayQueue(animQueue, onFinish, speed));
    }

    IEnumerator  PlayQueue(List<string> animQueue, Callback onFinish, float speed = 1f)
    {
        for(int i=0;i<animQueue.Count;i++)
        {
            Play(animQueue[i], null, false, speed);
            while (!mIsFinish)
            {
                yield return null;
            }
        }
        if (onFinish != null)
        {
            onFinish.Invoke();
        }
    }

    public void  Break()
    {
        mCurFinishCallback = null;
        if (string.IsNullOrEmpty(mCurAnimName))
        {
            return;
        }
        if (mAnimator != null)
        {
            mAnimator.SetBool(mCurAnimName, false);
        }
    }

    public void  SetSpeed(float speed)
    {
        mPlaySpeed = speed;
    }

    public float GetLength(string animName)
    {
        if (mAnimClips.ContainsKey(animName))
        {
            return mAnimClips[animName].length;
        }
        return 0;
    }

    public float GetSpeed()
    {
        return mPlaySpeed;
    }

    static Dictionary<Animator, GTAction> mActions = new Dictionary<Animator, GTAction>();
    static List<GTAction>                 mDeleteList = new List<GTAction>();

    public static void     Update()
    {
        Dictionary<Animator, GTAction>.Enumerator em = mActions.GetEnumerator();
        while (em.MoveNext())
        {
            Animator animator = em.Current.Key;
            if (animator == null)
            {
                mDeleteList.Add(em.Current.Value);
            }
            else
            {
                em.Current.Value.Execute();
            }
        }
        em.Dispose();
        while (mDeleteList.Count > 0)
        {
            Animator animator = mDeleteList[0].mAnimator;
            mActions.Remove(animator);
            mDeleteList.RemoveAt(0);
        }
    }

    public static GTAction Get(Animator animator)
    {
        if (animator == null)
        {
            return null;
        }
        GTAction action = null;
        mActions.TryGetValue(animator, out action);
        if (action == null)
        {
            action = new GTAction(animator);
            mActions[animator] = action;
        }
        return action;
    }

    public static GTAction Get(GameObject model)
    {
        if (model == null)
        {
            return null;
        }
        Animator animator = model.GetComponent<Animator>();
        return Get(animator);
    }

    public static void     Remove(GTAction action)
    {
        if (action == null)
        {
            return;
        }
        mDeleteList.Add(action);
    }
}