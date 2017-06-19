using UnityEngine;
using System;
using System.Collections.Generic;

public class GTEventCenter
{
    public class EventData
    {
        public Delegate Del;
        public object   Obj;
    }

    static Dictionary<GTEventID, Delegate>  mEvents     = new Dictionary<GTEventID, Delegate>();
    static Dictionary<GTEventID, EventData> mEventDatas = new Dictionary<GTEventID, EventData>();

    static void OnListenerAdding(GTEventID e, Delegate d)
    {
        if (!mEvents.ContainsKey(e))
        {
            mEvents.Add(e, null);
        }
        Delegate ed = mEvents[e];
        if (ed != null && d.GetType() != ed.GetType())
        {
            string error = string.Format("添加事件监听错误，EventID:{0}，添加的事件{1},已存在的事件{2}", e.ToString(), d.GetType().Name, ed.GetType().Name);
            Debug.LogError(error);
        }
    }

    static void OnListenerRemoved(GTEventID e)
    {
        if (mEvents[e] == null)
        {
            mEvents.Remove(e);
        }
    }

    public static void AddHandler(GTEventID e, Callback handler)
    {
        OnListenerAdding(e,handler);
        mEvents[e] = (Callback)mEvents[e] + handler;
    }

    public static void AddHandler<T>(GTEventID e, Callback<T> handler)
    {
        OnListenerAdding(e, handler);
        mEvents[e] = (Callback<T>)mEvents[e] + handler;
    }

    public static void AddHandler<T, U>(GTEventID e, Callback<T, U> handler)
    {
        OnListenerAdding(e, handler);
        mEvents[e] = (Callback<T, U>)mEvents[e] + handler;
    }

    public static void AddHandler<T, U, V>(GTEventID e, Callback<T, U, V> handler)
    {
        OnListenerAdding(e, handler);
        mEvents[e] = (Callback<T, U, V>)mEvents[e] + handler;
    }

    public static void AddHandler<T, U, V, X>(GTEventID e, Callback<T, U, V, X> handler)
    {
        OnListenerAdding(e, handler);
        mEvents[e] = (Callback<T, U, V, X>)mEvents[e] + handler;
    }

    public static void DelHandler(GTEventID e, Callback handler)
    {
        if (mEvents.ContainsKey(e) == false)
        {
            return;
        }
        mEvents[e] = (Callback)mEvents[e] - handler;
        OnListenerRemoved(e);
    }

    public static void DelHandler<T>(GTEventID e, Callback<T> handler)
    {
        if (mEvents.ContainsKey(e) == false)
        {
            return;
        }
        mEvents[e] = (Callback<T>)mEvents[e] - handler;
        OnListenerRemoved(e);
    }

    public static void DelHandler<T, U>(GTEventID e, Callback<T, U> handler)
    {
        if(mEvents.ContainsKey(e) == false)
        {
            return;
        }
        mEvents[e] = (Callback<T, U>)mEvents[e] - handler;
        OnListenerRemoved(e);
    }

    public static void DelHandler<T, U, V>(GTEventID e, Callback<T, U, V> handler)
    {
        if (mEvents.ContainsKey(e) == false)
        {
            return;
        }
        mEvents[e] = (Callback<T, U, V>)mEvents[e] - handler;
        OnListenerRemoved(e);
    }

    public static void DelHandler<T, U, V, X>(GTEventID e, Callback<T, U, V, X> handler)
    {
        if (mEvents.ContainsKey(e) == false)
        {
            return;
        }
        mEvents[e] = (Callback<T, U, V, X>)mEvents[e] - handler;
        OnListenerRemoved(e);
    }

    public static void FireEvent(GTEventID e)
    {
        Delegate d;
        if (mEvents.TryGetValue(e, out d))
        {
            Callback callback = d as Callback;

            if (callback != null)
            {
                callback();
            }
        }
    }

    public static void FireEvent<T>(GTEventID e, T arg1)
    {
        Delegate d;
        if (mEvents.TryGetValue(e, out d))
        {
            Callback<T> callback = d as Callback<T>;

            if (callback != null)
            {
                callback(arg1);
            }
        }
    }

    public static void FireEvent<T, U>(GTEventID e, T arg1, U arg2)
    {
        Delegate d;
        if (mEvents.TryGetValue(e, out d))
        {
            Callback<T, U> callback = d as Callback<T, U>;

            if (callback != null)
            {
                callback(arg1, arg2);
            }
        }
    }

    public static void FireEvent<T, U, V>(GTEventID e, T arg1, U arg2, V arg3)
    {
        Delegate d;
        if (mEvents.TryGetValue(e, out d))
        {
            Callback<T, U, V> callback = d as Callback<T, U, V>;

            if (callback != null)
            {
                callback(arg1, arg2, arg3);
            }
        }
    }

    public static void FireEvent<T, U, V, X>(GTEventID e, T arg1, U arg2, V arg3, X arg4)
    {
        Delegate d;
        if (mEvents.TryGetValue(e, out d))
        {
            Callback<T, U, V, X> callback = d as Callback<T, U, V, X>;

            if (callback != null)
            {
                callback(arg1, arg2, arg3, arg4);
            }
        }
    }
}