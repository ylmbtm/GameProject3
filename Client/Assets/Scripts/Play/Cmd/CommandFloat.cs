using UnityEngine;
using System.Collections;

/// <summary>
/// 浮空
/// </summary>
public class CommandFloat : CommandAct
{
    public override Resp Do()
    {
        CmdHandler<CommandFloat> call = Del as CmdHandler<CommandFloat>;
        return call == null ? Resp.TYPE_NO : call(this);
    }
}
