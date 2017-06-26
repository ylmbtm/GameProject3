using UnityEngine;
using System.Collections;

public class CharacterRideIdleFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoRideIdle();
    }
}
