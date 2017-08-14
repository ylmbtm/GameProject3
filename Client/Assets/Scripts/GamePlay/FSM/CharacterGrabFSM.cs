using UnityEngine;
using System.Collections;

public class CharacterGrabFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoGrab(Cmd as CommandGrab);
    }
}
