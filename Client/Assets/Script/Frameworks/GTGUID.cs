using UnityEngine;
using System.Collections;

public class GTGUID
{
    static int GUID = 10000001;

    public static int NewGUID()
    {
        GUID++;
        return GUID;
    }
}
