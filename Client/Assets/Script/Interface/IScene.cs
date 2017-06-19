using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public abstract class IScene : IState<GTLauncher, ESceneType> , ISceneListener
{
    public override void Enter()       { }
    public override void Execute()     { }
    public override void Exit()        { }
    public override void Stop()       { }
    public virtual  void InitWindows() { }
}
