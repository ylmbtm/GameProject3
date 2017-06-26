using UnityEngine;
using System.Collections;
using System;

public class UIPartnerBattle : GTWindow
{
    private GameObject   btnClose;
    private GameObject[] btnSelects;
    private int mSelectID;

    public UIPartnerBattle()
    {
        Type = EWindowType.DIALOG;
        mResPath = "Partner/UIPartnerBattle";
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnClose = pivot.Find("Btn_Close").gameObject;
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
                PartnerService.Instance.TryChangePartner(pos, mSelectID);
            };
        }
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_CHANGE_PARTNER, OnRecvChangePartner);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_CHANGE_PARTNER, OnRecvChangePartner);
    }

    private void OnRecvChangePartner(int arg1, int arg2)
    {
        Hide();
    }

    public void SetID(int id)
    {
        mSelectID = id;
    }

    protected override void OnEnable()
    {
        
    }

    protected override void OnClose()
    {
        
    }
}
