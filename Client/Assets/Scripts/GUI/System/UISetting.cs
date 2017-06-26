using UnityEngine;
using System.Collections;
using System;

public class UISetting : GTWindow
{
    private Transform  pivot;
    private GameObject btnClose;
    private GameObject btnSure;
    private GameObject btnLogout;

    private UIToggle musicToggle;
    private UIToggle soundToggle;
    private UIToggle effectToggle;

    public UISetting()
    {
        Type = EWindowType.DIALOG;
        mResident = false;
        mResPath = "System/UISetting";
    }

    protected override void OnAwake()
    {
        this.pivot        = this.transform.Find("Pivot");
        this.btnClose     = this.pivot.Find("Btn_Close").gameObject;
        this.btnLogout    = this.pivot.Find("Btn_Logout").gameObject;
        this.btnSure      = this.pivot.Find("Btn_Sure").gameObject;
        this.musicToggle  = this.pivot.Find("Music").GetComponent<UIToggle>();
        this.soundToggle  = this.pivot.Find("Sound").GetComponent<UIToggle>();
        this.effectToggle = this.pivot.Find("Effect").GetComponent<UIToggle>();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick  = OnCloseClick;
        UIEventListener.Get(btnLogout).onClick = OnLogoutClick;
        UIEventListener.Get(btnSure).onClick   = OnSureClick;
        UIEventListener.Get(musicToggle.gameObject).onClick = OnMusicToggle;
        UIEventListener.Get(soundToggle.gameObject).onClick = OnSoundToggle;
        UIEventListener.Get(effectToggle.gameObject).onClick = OnEffectToggle;
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
        InitView();
    }

    private void InitView()
    {
        this.musicToggle.value  = GTAudioManager.Instance.MusicActive;
        this.soundToggle.value  = GTAudioManager.Instance.SoundActive;
    }

    private void OnSureClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    private void OnLogoutClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTLauncher.Instance.LoadScene(GTSceneKey.SCENE_LOGIN);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    private void OnEffectToggle(GameObject go)
    {

    }

    private void OnSoundToggle(GameObject go)
    {
        GTAudioManager.Instance.SetSoundActive(!GTAudioManager.Instance.SoundActive);
    }

    private void OnMusicToggle(GameObject go)
    {
        GTAudioManager.Instance.SetMusicActive(!GTAudioManager.Instance.MusicActive);
        if (GTAudioManager.Instance.MusicActive)
        {
            DScene db = ReadCfgScene.GetDataById(GTLauncher.CurSceneID);
            GTAudioManager.Instance.PlayMusic(db.SceneMusic);
        }
    }
}
