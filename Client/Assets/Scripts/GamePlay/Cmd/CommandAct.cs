using UnityEngine;
using System.Collections;
using System;

public class CommandAct : ICommand
{
    public Callback ExitCallback;
    public Callback StopCallback;

    public override Resp Do()
    {
        return Resp.TYPE_NO;
    }

    public CommandAct Update(Callback exit, Callback stop)
    {
        this.ExitCallback = exit;
        this.StopCallback = stop;
        return this;
    }
}
