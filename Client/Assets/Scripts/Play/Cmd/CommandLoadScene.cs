using UnityEngine;
using System.Collections;

/// <summary>
/// 场景加载
/// </summary>
public class CommandLoadScene : ICommand
{
    public int SceneID;

    public override Resp Do()
    {
        CmdHandler<CommandLoadScene> call = Del as CmdHandler<CommandLoadScene>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandLoadScene Update(int sceneID)
    {
        this.SceneID = sceneID;
        return this;
    }
}
