
using UnityEngine;
using System.Collections;
using System;

public class UILogin : GTWindow
{

    private GameObject       btnNotice;
    private GameObject       btnLoginGame;
    private GameObject       btnAccount;
    private UILabel          mVersion;
    private CharacterAvatar  mLoginMount;


    public UILogin()
    {
        mResident = false;
        mResPath = "Login/UILogin";
        Type = EWindowType.WINDOW;
    }

    protected override void OnAwake()
    {
        btnAccount = transform.Find("Buttons/Btn_Account").gameObject;
        btnNotice = transform.Find("Buttons/Btn_Notice").gameObject;
        btnLoginGame = transform.Find("Buttons/Btn_LoginGame").gameObject;
        mVersion = transform.Find("Bottom/Version").GetComponent<UILabel>();
    }

    private void InitModel()
    {
        KTransform param = KTransform.Create(Vector3.zero,Vector3.zero);
        Camera camera = GTCameraManager.Instance.MainCamera;
        camera.transform.position    = new Vector3(0.2214f, 1.5794f, 41.2714f);
        camera.transform.eulerAngles = new Vector3(-23.7111f, -179.525f, 0.01245f);
        camera.fieldOfView = 46.6f;
        camera.renderingPath = RenderingPath.DeferredLighting;
    }

    private void InitView()
    {
        mVersion.text = GTTools.Format("版本号：{0}", Application.version);
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnAccount).onClick = OnAccountClick;
        UIEventListener.Get(btnNotice).onClick = OnNoticeClick;
        UIEventListener.Get(btnLoginGame).onClick = OnLoginGameClick;
    }

    private void OnLoginGameClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        LoginService.Instance.TryLoginGame();
    }

    private void OnAccountClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnNoticeClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_LOGINGAME_CALLBACK, OnRecvLoginGame);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_LOGINGAME_CALLBACK, OnRecvLoginGame);
    }

    protected override void OnEnable()
    {
        InitModel();
        InitView();
    }

    protected override void OnClose()
    {
        GTCameraManager.Instance.RevertMainCamera();
    }

    private void OnRecvLoginGame()
    {
        Hide();
    }
}
