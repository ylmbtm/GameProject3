using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public sealed class SceneRole : IScene
{
    public override void InitWindows()
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UI_CREATEROLE);
    }

    public override void Exit()
    {
        base.Exit();
        GuideManager.Instance.Init();
    }
}
