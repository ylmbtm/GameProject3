using UnityEngine;
using System.Collections;
using System.Collections.Generic;


public class GTLog
{
    static HashSet<string> mLogSets = new HashSet<string>();

    static GTLog()
    {

    }

    public static void Open(string tag)
    {
        mLogSets.Add(tag);
    }

    public static void D(string tag, object obj)
    {
        if (!mLogSets.Contains(tag))
        {
            return;
        }
        Debug.Log(obj);
    }

    public static void G(string tag, object obj)
    {
        if (!mLogSets.Contains(tag))
        {
            return;
        }
        Debug.Log(string.Format("<color=#00FF00>{0}</color>", obj));
    }

    public static void W(string tag, object obj)
    {
        if (!mLogSets.Contains(tag))
        {
            return;
        }
        Debug.LogWarning(obj);
    }

    public static void E(string tag, object obj)
    {
        if (!mLogSets.Contains(tag))
        {
            return;
        }
        Debug.LogError(obj);
    }
}