
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
    private GameObject       btnAccountReg;
    private GameObject       btnAccountLogin;

    private UILabel          mCurServerName;

    UIInput                 m_AccountInput;
    UIInput                 m_PasswordInput;
    UIInput                 m_PasswordInput2;

    public UILogin()
    {
        mResident = false;
        mResPath = "Login/UILogin";
        Type = EWindowType.WINDOW;
    }

    protected override void OnAwake()
    {
        btnAccount = transform.Find("UI_Role_Login/Buttons/Btn_Account").gameObject;
        btnNotice = transform.Find("UI_Role_Login/Buttons/Btn_Notice").gameObject;
        mVersion = transform.Find("UI_Role_Login/Bottom/Version").GetComponent<UILabel>();
        mCurServerName = transform.Find("UI_Role_Login/TB_ServerName").GetComponent<UILabel>();
        btnLoginGame = transform.Find("UI_Role_Login/Buttons/Btn_LoginGame").gameObject;
        transform.Find("UI_Role_Login").gameObject.SetActive(false);

        btnAccountReg = transform.Find("UI_Account_Login/Btn_Account_Reg").gameObject;
        btnAccountLogin = transform.Find("UI_Account_Login/Btn_Account_Login").gameObject;

        m_AccountInput = transform.Find("UI_Account_Login/Account").GetComponent<UIInput>();
        m_PasswordInput = transform.Find("UI_Account_Login/Password").GetComponent<UIInput>();
        m_PasswordInput2 = transform.Find("UI_Account_Login/Password2").GetComponent<UIInput>();
        //transform.Find("UI_Account_Login/Password2").gameObject.SetActive(false);
        transform.Find("UI_Account_Login").gameObject.SetActive(true);
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
       // mVersion.text = GTTools.Format("版本号：{0}", Application.version);
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnAccount).onClick = OnAccountClick;
        UIEventListener.Get(btnNotice).onClick = OnNoticeClick;
        UIEventListener.Get(btnLoginGame).onClick = OnLoginGameClick;
        UIEventListener.Get(btnAccountReg).onClick = OnAccountRegClick;
        UIEventListener.Get(btnAccountLogin).onClick = OnAccountLoginClick;
    }

    private void OnLoginGameClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);

        //进入游戏，就选择服务器
        LoginService.Instance.TrySelectServer( LoginService.Instance.m_CurServerID);
    }

    private void OnAccountClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnNoticeClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnAccountRegClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);

        if (m_PasswordInput.text != m_PasswordInput2.text)
        {
            return;
        }

        LoginService.Instance.TryAccountReg(m_AccountInput.text, m_PasswordInput.text);
    }

    private void OnAccountLoginClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        LoginService.Instance.TryAccountLogin(m_AccountInput.text, m_PasswordInput.text);
    }


    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_LOGINGAME_CALLBACK, OnRecvLoginGame);
        GTEventCenter.AddHandler(GTEventID.TYPE_ACCLOGIN_CALLBACK, OnRecvAccountLogin);
        
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_LOGINGAME_CALLBACK, OnRecvLoginGame);
        GTEventCenter.DelHandler(GTEventID.TYPE_ACCLOGIN_CALLBACK, OnRecvAccountLogin);
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

    private void OnRecvAccountLogin()
    {
        transform.Find("UI_Account_Login").gameObject.SetActive(false);
        transform.Find("UI_Role_Login").gameObject.SetActive(true);
        mCurServerName.text = LoginService.Instance.m_CurServerName;
    }

    private void OnRecvLoginGame()
    {
        Hide();
    }
}
