using UnityEngine;
using System.Collections;

public class CharacterEmptyFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        Owner.DoEmpty();
    }
}
