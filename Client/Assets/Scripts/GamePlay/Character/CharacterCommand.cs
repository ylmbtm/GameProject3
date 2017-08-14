using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class CharacterCommand
{
    private Dictionary<Type, ICommand> mCmds = new Dictionary<Type, ICommand>();

    public void Add<T>(CmdHandler<T> del) where T : ICommand, new()
    {
        T t = Activator.CreateInstance<T>();
        t.Del = del;
        mCmds.Add(t.GetType(), t);
    }

    public T    Get<T>() where T : ICommand
    {
        ICommand iCmd = null;
        mCmds.TryGetValue(typeof(T), out iCmd);
        if (iCmd == null)
        {
            GTLog.E(GTLogTag.TAG_ACTOR, string.Format("Not Register ICmd={0}", typeof(T)));
            return null;
        }
        T t = (T)iCmd;
        return t;
    }
}
