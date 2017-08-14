using UnityEngine;
using System.Collections;
using System;
using Protocol;

public class NetworkCtrl
{
    public void AddHandlers()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_NETWORK_CONNECT_SUCCESS,                 OnConnectSuccess);
        GTEventCenter.AddHandler<MessageRetCode>(GTEventID.TYPE_NETWORK_CONNECT_FAIL,    OnConnectFail);
        GTEventCenter.AddHandler(GTEventID.TYPE_NETWORK_CONNECT,                         OnConnectStart);
    }

    public void DelHandlers()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_NETWORK_CONNECT_SUCCESS,                 OnConnectSuccess);
        GTEventCenter.DelHandler<MessageRetCode>(GTEventID.TYPE_NETWORK_CONNECT_FAIL,    OnConnectFail);
        GTEventCenter.DelHandler(GTEventID.TYPE_NETWORK_CONNECT,                         OnConnectStart);
    }

    private void OnConnectStart()
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UINetWaiting);
    }

    private void OnConnectSuccess()
    {
        GTWindowManager.Instance.HideWindow(EWindowID.UINetWaiting);
    }

    private void OnConnectFail(MessageRetCode arg1)
    {
        GTWindowManager.Instance.HideWindow(EWindowID.UINetWaiting);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIMessageBoxForNetwork);
        UIMessageBoxForNetwork dialog = (UIMessageBoxForNetwork)GTWindowManager.Instance.GetWindow(EWindowID.UIMessageBoxForNetwork);
        dialog.ShowView(string.Empty, "网络连接失败，请稍后重试", NetworkManager.Instance.ReConnect, () =>
        {
#if UNITY_EDITOR
            UnityEditor.EditorApplication.isPlaying = false;
#else
            Application.Quit();
#endif
        });
    }
}
