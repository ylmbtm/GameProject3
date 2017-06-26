using UnityEngine;
using System.Collections;

/// <summary>
/// 隐身
/// </summary>
public class CommandStealthBegin : ICommand
{
    public float LastTime;

    public override Resp Do()
    {
        CmdHandler<CommandStealthBegin> call = Del as CmdHandler<CommandStealthBegin>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandStealthBegin Update(float lastTime)
    {
        this.LastTime =lastTime;
        return this;
    }
}
