using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public sealed class SceneLoading : IScene
{ 
    public float              mWaitTime = 0.1f;
    public float              mWaitTimer = 0f;
    public bool               mLoading = false;
    public int                mLoadingSceneId;
    public UILoading          mLoadingWindow;
    public AsyncOperation     mAsync;
    public Callback           mCallback;

    public override void Enter()
    {
        base.Enter();
        CommandLoadScene ev = Cmd as CommandLoadScene;
        if (GTWorld.Instance.Bie != null)
        {
            GTWorld.Instance.Bie.PauseGuide = true;
        }
        GTWindowManager. Instance.Release();
        GTWorld.         Instance.Release();
        GTPoolManager.   Instance.Release();
        GTWindowManager. Instance.OpenWindow(EWindowID.UILoading);

        mLoadingSceneId = ev.SceneID;
        mLoadingWindow  = (UILoading)GTWindowManager.Instance.GetWindow(EWindowID.UILoading);
        mWaitTime       = Time.realtimeSinceStartup;
    }

    public override void Execute()
    {
        if (mLoading == false)
        {
            if (Time.realtimeSinceStartup - mWaitTime > 0.1f)
            {
                GTCoroutinueManager.Instance.StartCoroutine(DoLoadScene());
                mLoading = true;
            }
        }
        if (mAsync != null)
        {
            if (mLoadingWindow.transform != null)
            {
                mLoadingWindow.UpdateProgress(mAsync.progress);
            }
            if (mAsync.isDone)
            {
                mLoading = false;
                mAsync = null;
                OnSceneWasLoaded();
            }
        }    
    }

    public override void Exit()
    {
        mWaitTimer = 0;
        mLoading = false;
        mAsync = null;
        GTCoroutinueManager.Instance.StartCoroutine(DoFadeScene());
    }

    void OnSceneWasLoaded()
    {
        DCopy db = ReadCfgCopy.GetDataById(mLoadingSceneId);
        GTAudioManager.Instance.PlayMusic(db.SceneMusic);
        GTLauncher.    Instance.ChangeState(GTLauncher.Instance.NextCopyType,Cmd);
        if (db.CopyType == ECopyType.TYPE_LOGIN || 
            db.CopyType == ECopyType.TYPE_ROLE)
        {
            GTWorld.Instance.ResetGuide();
            GTWorld.Instance.EnterWorld(mLoadingSceneId);
        }
    }

    void OnSceneWasFadeOut()
    {
        GTGlobal.CurCopyID = mLoadingSceneId;
        GTWindowManager.Instance.HideWindow(EWindowID.UILoading);
        if (GTWorld.Instance.Bie != null)
        {
            GTWorld.Instance.Bie.PauseGuide = false;
        }
    }

    IEnumerator DoLoadScene()
    {
        if (mAsync == null)
        {
            mAsync = GTLauncher.Instance.LoadLevelById(mLoadingSceneId);
        }
        mAsync.allowSceneActivation = false;
        while (mAsync.progress < 0.9f)
        {
            yield return new WaitForEndOfFrame();
        }
        mAsync.allowSceneActivation = true;
    }

    IEnumerator DoFadeScene()
    {
        yield return new WaitForSeconds(0.2f);
        if (mCallback != null)
        {
            mCallback.Invoke();
            mCallback = null;
        }
        yield return new WaitForSeconds(0.8f);
        OnSceneWasFadeOut();
    }
}
