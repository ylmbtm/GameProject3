using UnityEngine;
using System.Collections;

public class CharacterTalkFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        Owner.DoTalk(Cmd as CommandTalk);
    }
}
