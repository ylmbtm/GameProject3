using UnityEngine;
using System.Collections;

/// <summary>
/// 转向
/// </summary>
public class CommandTurn : ICommand
{
    public Vector3  LookDirection;
    public Callback OnFinish;

    public override Resp Do()
    {
        CmdHandler<CommandTurn> call = Del as CmdHandler<CommandTurn>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandTurn Update(Vector3 lookDirection, Callback onFinish)
    {
        this.LookDirection = lookDirection;
        this.OnFinish = onFinish;
        return this;
    }
}