using UnityEngine;
using System.Collections;

public class CharacterRideMoveFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoRideMove();
    }
}
