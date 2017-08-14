using UnityEngine;
using System.Collections;

public class CharacterFloatFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoFloatStart(Cmd as CommandFloat);
    }

    public override void Exit()
    {
        base.Exit();
        Owner.DoFloatLeave();
        CommandFloat c = Cmd as CommandFloat;
        if (c.ExitCallback != null)
        {
            c.ExitCallback();
            c.ExitCallback = null;
        }
    }

    public override void Stop()
    {
        base.Stop();
        CommandFloat c = Cmd as CommandFloat;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
