using UnityEngine;
using System.Collections;

public class CharacterSleepFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        Owner.DoSleep(Cmd as CommandSleep);
    }

    public override void Exit()
    {
        base.Exit();
        CommandSleep c = Cmd as CommandSleep;
        if (c.ExitCallback != null)
        {
            c.ExitCallback();
            c.ExitCallback = null;
        }
    }

    public override void Stop()
    {
        base.Stop();
        CommandSleep c = Cmd as CommandSleep;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
