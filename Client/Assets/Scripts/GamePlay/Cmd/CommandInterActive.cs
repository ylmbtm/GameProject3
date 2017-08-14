using UnityEngine;
using System.Collections;


/// <summary>
/// NPC交互
/// </summary>
public class CommandInterActive : ICommand
{
    public float    LastTime;
    public string   AnimName = "idle";
    public Callback OnFinish;

    public override Resp Do()
    {
        CmdHandler<CommandInterActive> call = Del as CmdHandler<CommandInterActive>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandInterActive Update(string pAnimName, Callback onFinish)
    {
        this.OnFinish = onFinish;
        this.AnimName = pAnimName;
        return this;
    }
}
