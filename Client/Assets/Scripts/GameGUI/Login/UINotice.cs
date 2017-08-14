using UnityEngine;
using System.Collections;
using System;

public class UINotice : GTWindow
{
    public UINotice()
    {
        Resident = false;
        Path = "Login/UINotice";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
    }

    private GameObject btnClose;


    protected override void OnAwake()
    {
        btnClose = transform.Find("Pivot/BtnClose").gameObject;
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
    }

    protected override void OnAddHandler()
    {

    }

    protected override void OnClose()
    {
        
    }

    protected override void OnDelHandler()
    {
        
    }

    protected override void OnEnable()
    {
        
    }

    private void OnCloseClick(GameObject go)
    {
        Hide();
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
    }
}
