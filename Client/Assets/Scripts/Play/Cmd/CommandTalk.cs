using UnityEngine;
using System.Collections;

/// <summary>
/// 交谈
/// </summary>
public class CommandTalk : ICommand
{
    public string  Word = string.Empty;

    public override Resp Do()
    {
        CmdHandler<CommandTalk> call = Del as CmdHandler<CommandTalk>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandTalk Update(string word)
    {
        this.Word     = word;
        return this;
    }
}
