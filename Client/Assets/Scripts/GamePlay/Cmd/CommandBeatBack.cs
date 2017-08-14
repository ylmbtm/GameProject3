using UnityEngine;
using System.Collections;
using System;

/// <summary>
/// 被击退
/// </summary>
public class CommandBeatBack : CommandAct
{
    public float MaxDis = 8;

    public override Resp Do()
    {
        CmdHandler<CommandBeatBack> call = Del as CmdHandler<CommandBeatBack>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandBeatBack Update(float maxDis)
    {
        this.MaxDis = maxDis;
        return this;
    }
}


