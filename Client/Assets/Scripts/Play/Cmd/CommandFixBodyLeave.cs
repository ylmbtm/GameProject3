using UnityEngine;
using System.Collections;

/// <summary>
/// 隐身
/// </summary>
public class CommandFixBodyLeave : ICommand
{
    public override Resp Do()
    {
        CmdHandler<CommandFixBodyLeave> call = Del as CmdHandler<CommandFixBodyLeave>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

}
