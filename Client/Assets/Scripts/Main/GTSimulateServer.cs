using UnityEngine;
using System.Collections;
using Protocol;

public class GTSimulateServer
{
    public static void CalcMainPropertys()
    {
        if(GTGlobal.IsAlong == false)
        {
            return;
        }
        GTGlobal.Main.CurEquips.Clear();
        for (int i = 0; i < 8; i++)
        {
            GTGlobal.Main.CurEquips.Add(0);
        }
        foreach (var e in DataDBSDressEquip.Dict)
        {
            GTGlobal.Main.CurEquips[e.Value.Pos - 1] = e.Value.Id;
        }
        foreach (var e in DataDBSDressGem.Dict)
        {
            GTGlobal.Main.CurGems[e.Value.Pos - 1] = e.Value.Id;
        }
        GTGlobal.Main.CurAttrs.Clear();
        GTGlobal.Main.CurAttrs.AddRange(GTAttrHelper.GetPropertys(GTGlobal.Main).Values);
    }
}
