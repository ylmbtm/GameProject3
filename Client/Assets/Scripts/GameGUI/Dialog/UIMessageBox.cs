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
        Resident = true;
        Path = "Common/UIMessageBox";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
        ShowMode = EWindowShowMode.DoNothing;
    }

    protected override void OnAwake()
    {
        this.pivot      = transform.Find("Pivot");
        this.boxTitle   = pivot.Find("Title").GetComponent<UILabel>();
        this.boxContent = pivot.Find("Content").GetComponent<UILabel>();
        this.btnClose   = pivot.Find("Btn_Close").gameObject;
        this.btnOK      = pivot.Find("Btn_OK").gameObject;
        this.btnCancle  = pivot.Find("Btn_Cancle").gameObject;
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
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        if (mOKFunc != null)
        {
            mOKFunc();
            mOKFunc = null;
        }
        Hide();
    }

    private void OnBtnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
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
        this.boxContent.text = string.IsNullOrEmpty(content) ? string.Empty : content;
        this.mOKFunc = ok;
        this.mCancleFunc = cancle;
    }
}
