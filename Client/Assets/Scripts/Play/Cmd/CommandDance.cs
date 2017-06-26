using UnityEngine;
using System.Collections;

public class CommandDance :ICommand
{
    public override Resp Do()
    {
        CmdHandler<CommandDance> call = Del as CmdHandler<CommandDance>;
        return call == null ? Resp.TYPE_NO : call(this);
    }
}
