using UnityEngine;
using System.Collections;
using System;

public class UIMessageTip : GTWindow
{
    private EHUDToast mHUDWarning;
    private EHUDToast mHUDExp;

    public UIMessageTip()
    {
        Resident = true;
        Path = "Common/UIMessageTip";
        Type = EWindowType.Message;
        MaskType = EWindowMaskType.None;
        ShowMode = EWindowShowMode.DoNothing;
    }

    protected override void OnAwake()
    {
        mHUDWarning = transform.Find("HUDWarning").GetComponent<EHUDToast>();
        mHUDExp     = transform.Find("HUDExp").GetComponent<EHUDToast>();
    }

    public void ShowWarning(string msg)
    {
        mHUDWarning.Add(msg, Color.red, 0);
    }

    public void ShowExp(string msg)
    {
        mHUDExp.Add(msg, Color.green, 0);
    }

    protected override void OnAddButtonListener()
    {
           
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
}
