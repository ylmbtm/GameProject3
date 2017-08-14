using UnityEngine;
using System.Collections;

/// <summary>
/// 冰冻
/// </summary>
public class CommandFrost : CommandAct
{
    public float LastTime;

    public override Resp Do()
    {
        CmdHandler<CommandFrost> call = Del as CmdHandler<CommandFrost>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandFrost Update(float lastTime)
    {
        this.LastTime = lastTime;
        return this;
    }

    public CommandFrost Update(float lastTime, Callback exit, Callback stop)
    {
        this.ExitCallback = exit;
        this.StopCallback = stop;
        this.LastTime = lastTime;
        return this;
    }
}
