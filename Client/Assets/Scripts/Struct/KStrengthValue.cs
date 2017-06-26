using UnityEngine;
using System.Collections;

public class KStrengthValue
{
    public EAttr      Property;
    public int        Value;
    public int        UnlockLevel;

    public KStrengthValue(EAttr e, int v, int l)
    {
        Property    = e;
        Value       = v;
        UnlockLevel = l;
    }

    public KStrengthValue()
    {

    }
}
