using UnityEngine;
using System.Collections;

public class CharacterDeadFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoDead(Cmd as CommandDead);
    }
}
