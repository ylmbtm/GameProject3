using UnityEngine;
using System.Collections;

public class CharacterWoundFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoWound(Cmd as CommandWound);
    }

    public override void Exit()
    {
        base.Exit();
        CommandWound c = Cmd as CommandWound;
        if (c.ExitCallback != null)
        {
            c.ExitCallback();
            c.ExitCallback = null;
        }
    }

    public override void Stop()
    {
        base.Stop();
        CommandWound c = Cmd as CommandWound;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
