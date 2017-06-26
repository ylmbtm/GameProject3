using UnityEngine;
using System.Collections;

/// <summary>
/// 滚地
/// </summary>
public class CommandRoll : ICommand
{
    public float LastTime;

    public override Resp Do()
    {
        CmdHandler<CommandRoll> call = Del as CmdHandler<CommandRoll>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandRoll Update(float lastTime)
    {
        this.LastTime = lastTime;
        return this;
    }
}