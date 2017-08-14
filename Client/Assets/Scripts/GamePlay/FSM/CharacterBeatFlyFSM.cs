using UnityEngine;
using System.Collections;

public class CharacterBeatFlyFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        Owner.DoBeatFly(Cmd as CommandBeatFly);
    }

    public override void Exit()
    {
        base.Exit();
        CommandBeatFly c = Cmd as CommandBeatFly;
        if (c.ExitCallback != null)
        {
            c.ExitCallback();
            c.ExitCallback = null;
        }
    }

    public override void Stop()
    {
        base.Stop();
        CommandBeatFly c = Cmd as CommandBeatFly;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
