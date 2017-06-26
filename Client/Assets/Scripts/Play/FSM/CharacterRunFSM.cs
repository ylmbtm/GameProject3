using UnityEngine;
using System.Collections;

public class CharacterRunFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoRun(Cmd as CommandMove);
    }
}
