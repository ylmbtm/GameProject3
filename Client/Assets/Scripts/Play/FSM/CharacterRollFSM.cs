using UnityEngine;
using System.Collections;

public class CharacterRollFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoRoll(Cmd as CommandRoll);
    }
}
