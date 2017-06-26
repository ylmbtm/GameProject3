using UnityEngine;
using System.Collections;

public interface ITrigger
{
    void OnTriggerEnter(Collider other);
    void OnTriggerStay(Collider other);
    void OnTriggerExit(Collider other);
}
