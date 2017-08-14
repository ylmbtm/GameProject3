using UnityEngine;
using System.Collections;

public class CharacterParalyFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        Owner.DoParaly(Cmd as CommandParaly);
    }

    public override void Exit()
    {
        base.Exit();
        Owner.EnableAnimator(true);
        CommandParaly c = Cmd as CommandParaly;
        if (c.ExitCallback != null)
        {
            c.ExitCallback();
            c.ExitCallback = null;
        }
    }

    public override void Stop()
    {
        base.Stop();
        Owner.EnableAnimator(true);
        CommandParaly c = Cmd as CommandParaly;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
