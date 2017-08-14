using UnityEngine;
using System.Collections;

public class ESpin : MonoBehaviour
{
    public void OnSpin(Vector2 delta, float speed)
    {
        UICamera.currentTouch.clickNotification = UICamera.ClickNotification.None;
        transform.localRotation = Quaternion.Euler(0f, -0.5f * delta.x * speed, 0f) * transform.localRotation;
    }

    public static ESpin Get(GameObject go)
    {
        if (go == null) return null;
        ESpin spin = go.GET<ESpin>();
        return spin;
    }
}
