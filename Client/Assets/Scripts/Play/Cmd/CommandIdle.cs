using UnityEngine;
using System.Collections;

/// <summary>
/// 闲置命令
/// </summary>
public class CommandIdle : ICommand
{
    public override Resp Do()
    {
        CmdHandler<CommandIdle> call = Del as CmdHandler<CommandIdle>;
        return call == null ? Resp.TYPE_NO : call(this);
    }
}