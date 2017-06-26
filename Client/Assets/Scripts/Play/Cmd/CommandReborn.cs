using UnityEngine;
using System.Collections;

//重生
public class CommandReborn : ICommand
{
    public float LastTime;

    public override Resp Do()
    {
        CmdHandler<CommandReborn> call = Del as CmdHandler<CommandReborn>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandReborn Update(float lastTime)
    {
        this.LastTime = lastTime;
        return this;
    }
}
