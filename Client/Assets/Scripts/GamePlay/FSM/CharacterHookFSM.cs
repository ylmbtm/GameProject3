using UnityEngine;
using System.Collections;

public class CharacterHookFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        Owner.DoHook(Cmd as CommandHook);
    }

    public override void Exit()
    {
        base.Exit();
        Owner.EnableAnimator(true);
        CommandHook c = Cmd as CommandHook;
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
        CommandHook c = Cmd as CommandHook;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
