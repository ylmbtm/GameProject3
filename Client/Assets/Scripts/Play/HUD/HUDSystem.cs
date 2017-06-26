using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using LVL;
using System;

public class HUDSystem : IGameLoop
{
    private Dictionary<IEntiny, UIBoard> mBoards = new Dictionary<IEntiny, UIBoard>();
    private Transform                    mBoardContainerRoot;

    public UIBoard Create(IEntiny entiny)
    {
        UIBoard window = null;
        mBoards.TryGetValue(entiny, out window);
        if(window==null)
        {
            GameObject go = GTPoolManager.Instance.GetObject(GTPrefabKey.PRE_UIBOARD);
            window = go.GET<UIBoard>();
        }
        window.SetTarget(entiny.CacheTransform);
        window.SetPath(GTPrefabKey.PRE_UIBOARD);
        window.gameObject.GET<LookCamera>();
        window.SetVisable(false);
        mBoards.Add(entiny, window);
        return window;
    }

    public void Release(IEntiny entiny)
    {
        UIBoard window = null;
        mBoards.TryGetValue(entiny, out window);
        if (window != null)
        {
            window.Release();
            mBoards.Remove(entiny);
        }
    }

    public void Execute()
    {

    }

    public void Release()
    {
        Dictionary<IEntiny, UIBoard>.Enumerator em = mBoards.GetEnumerator();
        while(em.MoveNext())
        {
            UIBoard window = em.Current.Value;
            window.Release();
        }
        em.Dispose();
        mBoards.Clear();
        mBoardContainerRoot = null;
    }
}
