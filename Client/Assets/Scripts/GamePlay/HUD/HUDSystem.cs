using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using MAP;
using System;

public class HUDSystem : IGameLoop
{
    private Dictionary<IEntiny, HUDBoard> mBoards = new Dictionary<IEntiny, HUDBoard>();

    public HUDBoard Create(IEntiny entiny)
    {
        HUDBoard window = null;
        mBoards.TryGetValue(entiny, out window);
        if(window==null)
        {
            GameObject go = GTPoolManager.Instance.GetObject(GTPrefabKey.PRE_UIBOARD);
            window = go.GET<HUDBoard>();
        }
        window.SetTarget(entiny.CacheTransform);
        window.SetPath(GTPrefabKey.PRE_UIBOARD);
        window.SetVisable(false);
        mBoards.Add(entiny, window);
        return window;
    }

    public void Release(IEntiny entiny)
    {
        HUDBoard window = null;
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
        Dictionary<IEntiny, HUDBoard>.Enumerator em = mBoards.GetEnumerator();
        while(em.MoveNext())
        {
            HUDBoard window = em.Current.Value;
            window.Release();
        }
        em.Dispose();
        mBoards.Clear();
    }
}
