using System;
using System.Collections.Generic;
using UnityEngine;

public class GTTimerManager : GTSingleton<GTTimerManager>, IGame
{
    private Dictionary<int, GTTimer> mTimers = new Dictionary<int, GTTimer>();
    private List<GTTimer>            mAddBuffer = new List<GTTimer>();
    private List<int>                mDeleteBuffer = new List<int>();
    private int                      mIndex = 0;

    public GTTimer AddListener(float callTime, Callback callback, int tick = 1)
    {
        if (callback == null)
        {
            return null;
        }
        if (callTime <= 0)
        {
            return null;
        }
        mIndex++;
        GTTimer item = new GTTimer();
        item.key = mIndex;
        item.callTime = callTime;
        item.callback = callback;
        item.tick = tick;
        item.currTick = 0;
        item.startTime = Time.realtimeSinceStartup;
        item.currTime = item.startTime;
        mAddBuffer.Add(item);
        return item;
    }

    public void DelListener(Callback callback)
    {
        Dictionary<int, GTTimer>.Enumerator em = mTimers.GetEnumerator();
        while (em.MoveNext())
        {
            if(em.Current.Value.callback==callback)
            {
                mDeleteBuffer.Add(em.Current.Key);
            }
        }
        em.Dispose();
    }

    public void DelListener(GTTimer timer)
    {
        if (timer == null)
        {
            return;
        }
        mDeleteBuffer.Add(timer.key);
    }

    public void Startup()
    {

    }

    public void Execute()
    {
        for (int i = 0; i < mAddBuffer.Count; i++)
        {
            GTTimer item = mAddBuffer[i];
            mTimers.Add(item.key, item);
        }
        mAddBuffer.Clear();
        if(mTimers.Count > 0)
        {
            Dictionary<int, GTTimer>.Enumerator em = mTimers.GetEnumerator();
            while (em.MoveNext())
            {
                GTTimer item = em.Current.Value;
                item.currTime = Time.realtimeSinceStartup;
                if (Time.realtimeSinceStartup - item.startTime >= item.callTime)
                {
                    if (item.callback != null)
                    {
                        item.callback();
                    }
                    item.startTime = Time.realtimeSinceStartup;
                    if (item.tick > 0)
                    {
                        item.currTick++;
                        if (item.tick == item.currTick)
                        {
                            mDeleteBuffer.Add(item.key);
                        }
                    }
                }
                if (item.pause == true)
                {
                    mDeleteBuffer.Add(item.key);
                }
            }
            em.Dispose();
        }
        for (int i = 0; i < mDeleteBuffer.Count; i++)
        {
            mTimers.Remove(mDeleteBuffer[i]);
        }
        mDeleteBuffer.Clear();
    }

    public void Release()
    {
        mTimers.Clear();
        mAddBuffer.Clear();
        mDeleteBuffer.Clear();
    }
}