using UnityEngine;
using System.Collections;
using System;

public class UIReborn : GTWindow
{
    private GameObject btnClose;
    private GameObject btnReborn;

    public UIReborn()
    {
        Type = EWindowType.DIALOG;
        mResPath = "Home/UIReborn";
    }

    protected override void OnAwake()
    {
        btnReborn = transform.Find("Btn_Reborn").gameObject;
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnReborn).onClick = OnRebornClick;
    }

    private void OnRebornClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        CharacterManager.Main.Command.Get<CommandReborn>().Do();
        Hide();
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
