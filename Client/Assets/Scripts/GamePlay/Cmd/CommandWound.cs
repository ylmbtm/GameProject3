using UnityEngine;
using System.Collections;

/// <summary>
/// 受击
/// </summary>
public class CommandWound : CommandAct
{
    public override Resp Do()
    {
        CmdHandler<CommandWound> call = Del as CmdHandler<CommandWound>;
        return call == null ? Resp.TYPE_NO : call(this);
    }
}