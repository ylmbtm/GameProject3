using UnityEngine;
using System.Collections;

public class CharacterRebornFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoReborn(Cmd as CommandReborn);
    }
}
