using UnityEngine;
using System.Collections;
using DG.Tweening.Core;
using DG.Tweening;

public class CharacterJumpFSM : CharacterBaseFSM
{
    public override void Enter()
    {
        base.Enter();
        CommandJump cmd = Cmd as CommandJump;
        Owner.DoJump(cmd);
    }

}
