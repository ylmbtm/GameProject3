using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIServer : GTWindow
{
    private GameObject   btnClose;
    private GameObject   btnCurrServer;
    private UIScrollView scrollView;
    private UIGrid       grid;
    private GameObject   temp;
    private UILabel      currSeverLabel;

    public UIServer()
    {
        Resident = false;
        Path = "Login/UIServer";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
    }

    protected override void OnAwake()
    {
        btnClose = transform.Find("Pivot/BtnClose").gameObject;
        btnCurrServer = transform.Find("Pivot/BtnCurrServer").gameObject;
        scrollView = transform.Find("Pivot/View").GetComponent<UIScrollView>();
        grid = transform.Find("Pivot/View/Grid").GetComponent<UIGrid>();
        temp = transform.Find("Pivot/View/Temp").gameObject;
        temp.SetActive(false);
        currSeverLabel = btnCurrServer.transform.Find("Label").GetComponent<UILabel>();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnBtnCloseClick;
        UIEventListener.Get(btnCurrServer).onClick = OnBtnCurrServerClick;
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
        List<ClientServerNode> list = MLLogin.Instance.GetServerList();
        for (int i = 0; i < list.Count; i++)
        {
            GameObject item = NGUITools.AddChild(grid.gameObject, temp);
            item.SetActive(true);
            ClientServerNode data = list[i];
            item.transform.Find("Label").GetComponent<UILabel>().text = data.SvrName;
            UIEventListener.Get(item).onClick = (go) =>
            {
                MLLogin.Instance.SetCurrServer(data);
                currSeverLabel.text = data.SvrName;
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK2);
                GTWindowManager.Instance.GetWindow<UILogin>(EWindowID.UILogin).ShowCurrServer();
            };
        }
    }

    private void OnBtnCloseClick(GameObject go)
    {
        Hide();
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
    }

    private void OnBtnCurrServerClick(GameObject go)
    {
        Hide();
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
    }
}
