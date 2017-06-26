using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Threading;

public abstract class GTSingleton<T> where T : new()
{
    private static T _instance;
    private static object _lock = new object();

    public static T Instance
    {
        get
        {
            if (_instance == null)
            {
                object lockObject = _lock;
                Monitor.Enter(lockObject);
                try
                {
                    if (_instance == null)
                        _instance = Activator.CreateInstance<T>();
                }
                finally
                {
                    Monitor.Exit(lockObject);
                }
            }
            return _instance;
        }
    }

    public virtual void Init()
    {

    }
}