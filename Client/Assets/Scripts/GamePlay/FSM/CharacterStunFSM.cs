using UnityEngine;
using System.Collections;

public class CharacterStunFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        Owner.DoStun(Cmd as CommandStun);
    }

    public override void Exit()
    {
        base.Exit();
        CommandStun c = Cmd as CommandStun;
        if (c.ExitCallback != null)
        {
            c.ExitCallback();
            c.ExitCallback = null;
        }
    }

    public override void Stop()
    {
        base.Stop();
        CommandStun c = Cmd as CommandStun;
        if (c.StopCallback != null)
        {
            c.StopCallback();
            c.StopCallback = null;
        }
    }
}
