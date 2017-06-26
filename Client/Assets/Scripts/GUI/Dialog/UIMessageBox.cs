using UnityEngine;
using System.Collections;
using System;

public class UIMessageBox : GTWindow
{
    private Transform  pivot;
    private GameObject btnClose;
    private GameObject btnOK;
    private GameObject btnCancle;
    private UILabel    boxTitle;
    private UILabel    boxContent;

    private Callback   mOKFunc;
    private Callback   mCancleFunc;

    public UIMessageBox()
    {
        mResident = true;
        mResPath = "Common/UIMessageBox";
        Type = EWindowType.DIALOG;
    }

    protected override void OnAwake()
    {
        this.pivot      = transform.Find("Pivot");
        this.boxTitle   = pivot.Find("Background/TopBar/Label").GetComponent<UILabel>();
        this.boxContent = pivot.Find("Content").GetComponent<UILabel>();
        this.btnClose   = pivot.Find("BtnClose").gameObject;
        this.btnOK      = pivot.Find("BtnOK").gameObject;
        this.btnCancle  = pivot.Find("BtnCancle").gameObject;
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick  = OnBtnCloseClick;
        UIEventListener.Get(btnOK).onClick     = OnBtnOKClick;
        UIEventListener.Get(btnCancle).onClick = OnBtnCancleClick;
    }

    private void OnBtnCancleClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (mCancleFunc != null)
        {
            mCancleFunc();
            mCancleFunc = null;
        }
        Hide();
    }

    private void OnBtnOKClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (mOKFunc != null)
        {
            mOKFunc();
            mOKFunc = null;
        }
        Hide();
    }

    private void OnBtnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
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

    public void ShowView(string title, string content, Callback ok, Callback cancle = null)
    {
        this.boxTitle.text = string.IsNullOrEmpty(title) ? "温馨提示" : title;
        this.boxContent.text = string.IsNullOrEmpty(title) ? string.Empty : content;
        this.mOKFunc = ok;
        this.mCancleFunc = cancle;
    }
}
