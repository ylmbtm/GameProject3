using UnityEngine;
using System.Collections;

public class CharacterDanceFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoDance(Cmd as CommandDance);
    }
}
