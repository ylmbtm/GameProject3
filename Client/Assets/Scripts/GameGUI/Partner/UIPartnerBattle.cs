using UnityEngine;
using System.Collections;
using System;

public class UIPartnerBattle : GTWindow
{
    private GameObject   btnClose;
    private GameObject[] btnSelects;
    private int          mSelectID;

    public UIPartnerBattle()
    {
        Type = EWindowType.Window;
        Path = "Partner/UIPartnerBattle";
        MaskType = EWindowMaskType.BlackTransparent;
        ShowMode = EWindowShowMode.SaveTarget;
        Resident = false;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnClose = pivot.Find("BtnClose").gameObject;
        btnSelects = new GameObject[2];
        for (int i = 0; i < btnSelects.Length; i++)
        {
            btnSelects[i] = pivot.Find("Btn_" + (i + 1)).gameObject;
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        for (int i = 0; i < btnSelects.Length; i++)
        {
            GameObject btn = btnSelects[i];
            int pos = i + 1;
            UIEventListener.Get(btn).onClick = (GameObject go) =>
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                GTNetworkSend.Instance.TryChangePartner(pos, mSelectID);
            };
        }
    }

    protected override void OnEnable()
    {

    }

    protected override void OnClose()
    {

    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_PARTNER_CHANGE, OnRecvChangePartner);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_PARTNER_CHANGE, OnRecvChangePartner);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void OnRecvChangePartner(int arg1, int arg2)
    {
        Hide();
    }

    public void SetID(int id)
    {
        mSelectID = id;
    }
}