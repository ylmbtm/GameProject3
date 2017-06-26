using UnityEngine;
using System.Collections;

/// <summary>
/// 被击飞
/// </summary>
public class CommandBeatFly : CommandAct
{
    public Vector3 Dir;

    public override Resp Do()
    {
        CmdHandler<CommandBeatFly> call = Del as CmdHandler<CommandBeatFly>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandBeatFly Update(Vector3 dir)
    {
        this.Dir = dir;
        return this;
    }
}

