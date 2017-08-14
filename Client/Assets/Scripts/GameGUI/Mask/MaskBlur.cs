using UnityEngine;
using System.Collections;

public class MaskBlur : MaskGUI
{
    private void OnEnable()
    {
        if (GTCameraManager.Instance.MainCamera == null)
        {
            return;
        }
        ScreenBlurRadial blur = GTCameraManager.Instance.MainCamera.GetComponent<ScreenBlurRadial>();
        if (blur != null)
        {
            blur.enabled = true;
        }
    }

    private void OnDisable()
    {
        if (GTCameraManager.Instance.MainCamera == null)
        {
            return;
        }
        ScreenBlurRadial blur = GTCameraManager.Instance.MainCamera.GetComponent<ScreenBlurRadial>();
        if (blur != null)
        {
            blur.enabled = false;
        }
    }
}
