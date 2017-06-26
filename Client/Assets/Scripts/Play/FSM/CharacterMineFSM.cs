using UnityEngine;
using System.Collections;

public class CharacterMineFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoMine(Cmd as CommandMine);
    }
}
