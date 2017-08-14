using UnityEngine;
using System.Collections;
using System;

public class UIAccount : GTWindow
{
    private GameObject  btnClose;
    private GameObject  btnSure;
    private GameObject  btnGoRegister;
    private GameObject  btnCancel;
    private GameObject  btnRegister;
    private UIInput     usernameInput;
    private UIInput     passwordInput;
    private bool        showRegister;
    


    public UIAccount()
    {
        Resident = false;
        Path = "Login/UIAccount";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
    }

    protected override void OnAwake()
    {
        btnClose = transform.Find("Pivot/BtnClose").gameObject;
        btnSure = transform.Find("Pivot/BtnSure").gameObject;
        btnGoRegister = transform.Find("Pivot/BtnGoRegister").gameObject;
        btnRegister = transform.Find("Pivot/BtnRegister").gameObject;
        btnCancel = transform.Find("Pivot/BtnCancel").gameObject;
        usernameInput = transform.Find("Pivot/Username/Input").GetComponent<UIInput>();
        passwordInput = transform.Find("Pivot/Password/Input").GetComponent<UIInput>();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnSure).onClick = OnSureClick;
        UIEventListener.Get(btnGoRegister).onClick = OnGoRegisterClick;
        UIEventListener.Get(btnRegister).onClick = OnRegisterClick;
        UIEventListener.Get(btnCancel).onClick = OnBtnCancelClick;
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
        this.showRegister = string.IsNullOrEmpty(MLLogin.Instance.LastUsername);
        this.InitView();
    }

    private void OnCloseClick(GameObject go)
    {
        this.Hide();
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
    }

    private void OnBtnCancelClick(GameObject go)
    {
        this.showRegister = false;
        this.InitView();
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnRegisterClick(GameObject go)
    {
        string username = this.usernameInput.value;
        string password = this.passwordInput.value;
        GTNetworkSend.Instance.TryRegister(username, password);
        Hide();
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
    }

    private void OnGoRegisterClick(GameObject go)
    {
        this.showRegister = true;
        this.InitView();
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnSureClick(GameObject go)
    {
        MLLogin.Instance.LastUsername = this.usernameInput.value;
        MLLogin.Instance.LastPassword = this.passwordInput.value;
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void InitView()
    {
        if(showRegister)
        {
            btnSure.SetActive(false);
            btnGoRegister.SetActive(false);
            btnRegister.SetActive(true);
            btnCancel.SetActive(true);
            usernameInput.value = string.Empty;
            passwordInput.value = string.Empty;
        }
        else
        {
            btnSure.SetActive(true);
            btnGoRegister.SetActive(true);
            btnRegister.SetActive(false);
            btnCancel.SetActive(false);
            usernameInput.value = MLLogin.Instance.LastUsername;
            passwordInput.value = MLLogin.Instance.LastPassword;
        }
    }
}
