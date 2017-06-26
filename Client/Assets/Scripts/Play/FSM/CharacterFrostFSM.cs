using UnityEngine;
using System.Collections;

public class CharacterFrostFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        Owner.EnableAnimator(false);
    }

    public override void Exit()
    {
        base.Exit();
        Owner.EnableAnimator(true);
        CommandFrost c = Cmd as CommandFrost;
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
        CommandFrost c = Cmd as CommandFrost;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
