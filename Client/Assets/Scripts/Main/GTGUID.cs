using UnityEngine;
using System.Collections;

public class GTGUID
{
    public static ulong NewGUID()
    {
        GTGlobal.LocalData.GUID++;
        return GTGlobal.LocalData.GUID;
    }
}
