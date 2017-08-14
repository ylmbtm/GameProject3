using UnityEngine;
using System.Collections;

public class DStrengthValue
{
    public EAttr      Property;
    public int        Value;
    public int        UnlockLevel;

    public DStrengthValue(EAttr e, int v, int l)
    {
        Property    = e;
        Value       = v;
        UnlockLevel = l;
    }

    public DStrengthValue()
    {

    }
}
