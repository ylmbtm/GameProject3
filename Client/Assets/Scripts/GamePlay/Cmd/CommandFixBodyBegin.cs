using UnityEngine;
using System.Collections;
using System;

/// <summary>
/// 定身
/// </summary>
public class CommandFixBodyBegin : CommandAct
{
    public float LastTime;

    public override Resp Do()
    {
        CmdHandler<CommandFixBodyBegin> call = Del as CmdHandler<CommandFixBodyBegin>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandFixBodyBegin Update(float lastTime)
    {
        this.LastTime = lastTime;
        return this;
    }

    public CommandFixBodyBegin Update(float lastTime, Callback exit, Callback stop)
    {
        this.ExitCallback = exit;
        this.StopCallback = stop;
        this.LastTime = lastTime;
        return this;
    }
}
