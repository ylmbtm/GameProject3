using UnityEngine;
using System.Collections;

public class CharacterInterActiveFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoInterActive(Cmd as CommandInterActive);
    }
}
