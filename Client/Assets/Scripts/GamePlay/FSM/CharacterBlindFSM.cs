using UnityEngine;
using System.Collections;

public class CharacterBlindFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        Owner.DoBlind(Cmd as CommandBlind);
    }

    public override void Exit()
    {
        base.Exit();
        CommandBlind c = Cmd as CommandBlind;
        if (c.ExitCallback != null)
        {
            c.ExitCallback();
            c.ExitCallback = null;
        }
    }

    public override void Stop()
    {
        base.Stop();
        CommandBlind c = Cmd as CommandBlind;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
