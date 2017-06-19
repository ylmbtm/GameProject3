using UnityEngine;
using System.Collections;

public class GTTimer
{
    public int           key;
    public Callback      callback;
    public float         callTime;
    public float         startTime;
    public int           tick;
    public int           currTick;
    public float         currTime;
    public bool          pause = false;

    public float         GetLeftTime()
    {
        return currTime - startTime > 0 ? currTime - startTime : 0;
    }
}
