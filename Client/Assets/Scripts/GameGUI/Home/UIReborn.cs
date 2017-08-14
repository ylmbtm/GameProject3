using UnityEngine;
using System.Collections;
using System;

public class UIReborn : GTWindow
{
    private GameObject btnBackToCity;
    private GameObject btnReborn;

    public UIReborn()
    {
        Type = EWindowType.Window;
        Path = "Home/UIReborn";
        MaskType = EWindowMaskType.BlackTransparent;
        Resident = false;
        ShowMode = EWindowShowMode.DoNothing;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnReborn = pivot.Find("Btn_Reborn").gameObject;
        btnBackToCity = pivot.Find("Btn_BackToCity").gameObject;
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnReborn).onClick = OnRebornClick;
        UIEventListener.Get(btnBackToCity).onClick = OnBackToCityClick;
    }

    protected override void OnAddHandler()
    {
        
    }

    protected override void OnEnable()
    {
        
    }

    protected override void OnDelHandler()
    {
        
    }

    protected override void OnClose()
    {
        
    }

    private void OnBackToCityClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryReborn(0);
        Hide();
    }

    private void OnRebornClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryReborn(1);
        Hide();
    }
}
