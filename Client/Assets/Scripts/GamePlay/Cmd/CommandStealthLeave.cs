using UnityEngine;
using System.Collections;

/// <summary>
/// 隐身
/// </summary>
public class CommandStealthLeave : ICommand
{
    public override Resp Do()
    {
        CmdHandler<CommandStealthLeave> call = Del as CmdHandler<CommandStealthLeave>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

}
