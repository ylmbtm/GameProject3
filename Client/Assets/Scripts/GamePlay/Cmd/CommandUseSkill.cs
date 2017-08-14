using UnityEngine;
using System.Collections;

/// <summary>
/// 使用技能
/// </summary>
public class CommandUseSkill : CommandAct
{
    public ESkillPos Pos       = ESkillPos.Skill_0;
    public int       ID;


    public override Resp Do()
    {
        CmdHandler<CommandUseSkill> call = Del as CmdHandler<CommandUseSkill>;
        return call == null ? Resp.TYPE_NO : call(this);
    }

    public CommandUseSkill Update(ESkillPos pos)
    {
        this.Pos = pos;
        return this;
    }

    public CommandUseSkill Update(int id)
    {
        this.ID = id;
        return this;
    }
}
