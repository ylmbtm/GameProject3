using UnityEngine;
using System.Collections;

/// <summary>
/// 死亡
/// </summary>
public class CommandDead : ICommand
{
    public EDeadReason Reason;

    public override Resp Do()
    {
        CmdHandler<CommandDead> call = Del as CmdHandler<CommandDead>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandDead Update(EDeadReason reason)
    {
        this.Reason = reason;
        return this;
    }
}
