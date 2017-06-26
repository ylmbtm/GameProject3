using UnityEngine;
using System.Collections;

/// <summary>
/// 被抓取
/// </summary>
public class CommandGrab : ICommand
{
    public override Resp Do()
    {
        CmdHandler<CommandGrab> call = Del as CmdHandler<CommandGrab>;
        return call == null ? Resp.TYPE_NO : call(this);
    }
}

