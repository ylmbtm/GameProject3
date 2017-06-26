using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class EffectView : MonoBehaviour
{
    public int                Id;
    public int                GUID;
    public EffectUnit         Unit;

    void OnTriggerEnter(Collider other)
    {
        if (Unit != null && Unit.OnTriggerEnter != null)
        {
            Unit.OnTriggerEnter(other);
        }
    }
}
