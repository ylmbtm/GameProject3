using UnityEngine;
using System.Collections;

public class PreviewCamera : MonoBehaviour
{
    void Start()
    {
        gameObject.SetActive(GTGlobal.IsLaunched == false);
    }
}
