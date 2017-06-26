using UnityEngine;
using System.Collections;

public class CharacterSkillFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        Owner.DoUseSkill(Cmd as CommandUseSkill);
    }

    public override void Exit()
    {
        base.Exit();
        CommandUseSkill c = Cmd as CommandUseSkill;
        if (c.ExitCallback != null)
        {
            c.ExitCallback();
            c.ExitCallback = null;
        }
    }

    public override void Stop()
    {
        base.Stop();
        CommandUseSkill c = Cmd as CommandUseSkill;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
