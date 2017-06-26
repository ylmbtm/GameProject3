using UnityEngine;
using System.Collections;
using Protocol;

public class RelicsModule : GTSingleton<RelicsModule>
{
    public int GetExp(XRelics relics, int index)
    {
        switch(index)
        {
            case 1:
                return relics.CurExp1;
            case 2:
                return relics.CurExp2;
            case 3:
                return relics.CurExp3;
            default:
                return 0;
        }
    }
}
