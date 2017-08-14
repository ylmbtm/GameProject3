using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public sealed class SceneRole : IScene
{
    public override void OpenWindows()
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UICreateRole);
    }
}
