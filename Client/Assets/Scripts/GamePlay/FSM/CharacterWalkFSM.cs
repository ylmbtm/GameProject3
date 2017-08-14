using UnityEngine;
using System.Collections;

public class CharacterWalkFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoWalk(Cmd as CommandMove);
    }
}
