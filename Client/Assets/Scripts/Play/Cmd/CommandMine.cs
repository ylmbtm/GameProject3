using UnityEngine;
using System.Collections;

/// <summary>
/// 采集
/// </summary>
public class CommandMine : ICommand
{
    public float    LastTime;
    public Callback OnFinish;

    public override Resp Do()
    {
        CmdHandler<CommandMine> call = Del as CmdHandler<CommandMine>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandMine Update(Callback onFinish)
    {
        this.OnFinish = onFinish;
        return this;
    }
}
