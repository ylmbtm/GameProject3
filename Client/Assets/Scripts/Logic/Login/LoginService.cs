using UnityEngine;
using System.Collections;
using Protocol;
using System;

public class LoginService : GTSingleton<LoginService>
{
    public Int32 m_CurServerID;
    public string m_CurServerName;
    public UInt64 m_CurAccountID;
    public Int32 m_LoginCode;


    //账号注册
    public void TryAccountReg(string name, string password)
    {
        AccountRegReq Req = new AccountRegReq();
        Req.AccountName = name;
        Req.Password = password;
        NetworkManager.Instance.Send(MessageID.MSG_ACCOUNT_REG_REQ, Req, 0, 0);
    }

    public void TryAccountLogin(string name, string password)
    {
        AccountLoginReq Req = new AccountLoginReq();
        Req.AccountName = name;
        Req.Password = password;
        NetworkManager.Instance.Send(MessageID.MSG_ACCOUNT_LOGIN_REQ, Req, 0, 0);
    }

    public void TryGetSvrList()
    {
        ClientServerListReq Req = new ClientServerListReq();
        Req.AccountID = 1;
        Req.Channel = 2;
        Req.ClientVersion = 100001;
        NetworkManager.Instance.Send(MessageID.MSG_SERVER_LIST_REQ, Req, 0, 0);
    }

    public void TryCreateRole(string name, uint roletype)
    {
        if (string.IsNullOrEmpty(name))
        {
            GTItemHelper.ShowTip("名字不能为空");
            return;
        }

        RoleCreateReq req = new RoleCreateReq();
        req.Name = name;
        req.AccountID = 1;
        req.RoleType = roletype;
        NetworkManager.Instance.Send(MessageID.MSG_ROLE_CREATE_REQ, req, 0, 0);
    }

    public void TryGetRoleList()
    {
        RoleListReq req = new RoleListReq();
        req.AccountID = 1;
        req.LoginCode = 0x111;
        NetworkManager.Instance.Send(MessageID.MSG_ROLE_LIST_REQ, req, 0, 0);
    }

    public void TryEnterGame(UInt64 roleID)
    {
        RoleLoginReq req = new RoleLoginReq();
        req.AccountID = 1;
        req.RoleID = roleID;
        req.LoginCode = 0x111;
        NetworkManager.Instance.Send(MessageID.MSG_ROLE_LOGIN_REQ, req, 0, 0);
    }

    public void TryEnterScene(UInt64 roleID, UInt32 CopyID, Int32 ServerID)
    {
        EnterSceneReq req = new EnterSceneReq();
        req.RoleID = roleID;
        NetworkManager.Instance.Send(MessageID.MSG_ENTER_SCENE_REQ, req, (UInt64)CopyID, (UInt32)ServerID);
    }

    public void TrySelectServer(Int32 ServerID)
    {
        SelectServerReq Req = new SelectServerReq();
        Req.ServerID = ServerID;
        NetworkManager.Instance.Send(MessageID.MSG_SELECT_SERVER_REQ, Req, 0, 0);
    }
}
