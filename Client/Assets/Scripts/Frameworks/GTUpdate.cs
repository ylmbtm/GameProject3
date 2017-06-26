using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GTUpdate : GTSingleton<GTUpdate>
{
    private List<System.Action> mUpdateCallbacks = new List<System.Action>();

    public void AddListener(System.Action callback)
    {
        mUpdateCallbacks.Add(callback);
    }

    public void DelListener(System.Action callback)
    {
        mUpdateCallbacks.Remove(callback);
    }

    public void Execute()
    {
        for (int i = 0; i < mUpdateCallbacks.Count; i++)
        {
            mUpdateCallbacks[i].Invoke();
        }
    }
}
