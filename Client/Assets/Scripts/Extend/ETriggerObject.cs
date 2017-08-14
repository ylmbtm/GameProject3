using UnityEngine;
using System.Collections;
using System;

public class ETriggerObject : MonoBehaviour
{
    public Action<Collider> onTriggerEnter { get; set; }
    public Action<Collider> onTriggerStay  { get; set; }
    public Action<Collider> onTriggerExit  { get; set; }

    void OnTriggerEnter(Collider other)
    {
        if (onTriggerEnter != null)
        {
            onTriggerEnter(other);
        }
    }

    void OnTriggerStay(Collider other)
    {
        if (onTriggerStay != null)
        {
            onTriggerStay(other);
        }
    }

    void OnTriggerExit(Collider other)
    {
        if (onTriggerExit != null)
        {
            onTriggerExit(other);
        }
    }
}
