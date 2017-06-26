using UnityEngine;
using System.Collections;
using System;

public class UITaskInterActive : GTWindow
{
    private UISlider mProgress;
    private UILabel  mInterActiveName;
    private GameObject btnInterActive;
    private Callback mCallback;

    public UITaskInterActive()
    {
        Type = EWindowType.MSGTIP;
        mResPath = "Task/UITask";
    }


    protected override void OnAwake()
    {
        mProgress = transform.Find("Progress").GetComponent<UISlider>();
        mInterActiveName = transform.Find("Name").GetComponent<UILabel>();
        btnInterActive = transform.Find("Btn_InterActive").gameObject;
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnInterActive).onClick = OnInterActiveClick;
    }

    private void OnInterActiveClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (mCallback!=null)
        {
            mCallback();
            mCallback = null;
        }
    }

    public void UpdateProgress(float value)
    {
        mProgress.value = value;
    }

    public void Init(string pName,Callback pCallback)
    {
        mInterActiveName.text = pName;
        mProgress.value = 0;
        mCallback = pCallback;
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
        mCallback = null;
    }
}
