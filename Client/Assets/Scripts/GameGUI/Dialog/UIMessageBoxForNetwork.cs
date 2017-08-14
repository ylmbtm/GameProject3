using UnityEngine;
using System.Collections;
using System;

public class UIMessageBoxForNetwork : GTWindow
{
    private Transform  pivot;
    private GameObject btnOK;
    private GameObject btnCancle;
    private UILabel    boxTitle;
    private UILabel    boxContent;

    private Callback   mOKFunc;
    private Callback   mCancleFunc;

    public UIMessageBoxForNetwork()
    {
        Resident = true;
        Path = "Common/UIMessageBoxForNetwork";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
        ShowMode = EWindowShowMode.DoNothing;
    }

    protected override void OnAwake()
    {
        this.pivot      = transform.Find("Pivot");
        this.boxTitle   = pivot.Find("Title").GetComponent<UILabel>();
        this.boxContent = pivot.Find("Content").GetComponent<UILabel>();
        this.btnOK      = pivot.Find("Btn_OK").gameObject;
        this.btnCancle  = pivot.Find("Btn_Cancle").gameObject;
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnOK).onClick     = OnBtnOKClick;
        UIEventListener.Get(btnCancle).onClick = OnBtnCancleClick;
    }

    private void OnBtnCancleClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK2);
        if (mCancleFunc != null)
        {
            mCancleFunc();
            mCancleFunc = null;
        }
        Hide();
    }

    private void OnBtnOKClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK2);
        if (mOKFunc != null)
        {
            mOKFunc();
            mOKFunc = null;
        }
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
