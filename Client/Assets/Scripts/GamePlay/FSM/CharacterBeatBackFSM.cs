using UnityEngine;
using System.Collections;

public class CharacterBeatBackFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        Owner.DoBeatBack(Cmd as CommandBeatBack);
    }

    public override void Exit()
    {
        base.Exit();
        CommandBeatBack c = Cmd as CommandBeatBack;
        if (c.ExitCallback != null)
        {
            c.ExitCallback();
            c.ExitCallback = null;
        }
    }

    public override void Stop()
    {
        base.Stop();
        CommandBeatBack c = Cmd as CommandBeatBack;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
