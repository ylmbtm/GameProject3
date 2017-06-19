using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public abstract class IState<T, FSMID> where T : class
{
    public T             Owner    { get; set; }
    public FSMID         Fsm      { get; private set; }
    public ICommand      Cmd      { get; private set; }
    public float         LastTime { get; protected set; }

    public abstract void Enter();
    public abstract void Execute();
    public abstract void Exit();
    public abstract void Stop();

    public void SetCommand(ICommand cmd)
    {
        Cmd = cmd;
    }

    public void SetFSMID(FSMID fsmID)
    {
        Fsm = fsmID;
    }

    public void SetFSMTime(float lastTime)
    {
        LastTime = lastTime;
    }
}