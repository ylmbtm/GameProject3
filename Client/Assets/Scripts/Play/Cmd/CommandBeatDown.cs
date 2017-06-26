using UnityEngine;
using System.Collections;

/// <summary>
/// 被击倒
/// </summary>
public class CommandBeatDown : CommandAct
{
    public override Resp Do()
    {
        CmdHandler<CommandBeatDown> call = Del as CmdHandler<CommandBeatDown>;
        return call == null ? Resp.TYPE_NO : call(this);
    }
}
