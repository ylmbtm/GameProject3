using UnityEngine;
using System.Collections;
using System;
using Protocol;
using ProtoBuf;
using System.IO;

public class LoginCtrl :  ICtrl
{
    public void AddListener()
    {
        NetworkManager.AddListener(MessageID.MSG_ACCOUNT_REG_ACK,   OnAck_Register);
        NetworkManager.AddListener(MessageID.MSG_ACCOUNT_LOGIN_ACK, OnAck_AccountLogin);
        NetworkManager.AddListener(MessageID.MSG_SELECT_SERVER_ACK, OnAck_SelectServer);
        NetworkManager.AddListener(MessageID.MSG_ROLE_LIST_ACK,     OnAck_RoleList);
        NetworkManager.AddListener(MessageID.MSG_SERVER_LIST_ACK,   OnAck_GetServerList);
        NetworkManager.AddListener(MessageID.MSG_ROLE_CREATE_ACK,   OnAck_CreateRole);
        NetworkManager.AddListener(MessageID.MSG_ROLE_LOGIN_ACK,    OnAck_EnterGame);

        NetworkManager.AddListener(MessageID.MSG_NOTIFY_INTO_SCENE, OnAck_NotifyIntoScene);
        NetworkManager.AddListener(MessageID.MSG_ENTER_SCENE_ACK,   OnAck_EnterScene);
        
    }

    public void DelListener()
    {
        NetworkManager.DelListener(MessageID.MSG_ACCOUNT_REG_ACK,   OnAck_Register);
        NetworkManager.DelListener(MessageID.MSG_ACCOUNT_LOGIN_ACK, OnAck_AccountLogin);
        NetworkManager.DelListener(MessageID.MSG_SELECT_SERVER_ACK, OnAck_SelectServer);
        NetworkManager.DelListener(MessageID.MSG_ROLE_LIST_ACK,     OnAck_RoleList);
        NetworkManager.DelListener(MessageID.MSG_SERVER_LIST_ACK,   OnAck_GetServerList);
        NetworkManager.DelListener(MessageID.MSG_ROLE_CREATE_ACK,   OnAck_CreateRole);
        NetworkManager.DelListener(MessageID.MSG_ROLE_LOGIN_ACK,    OnAck_EnterGame);
        NetworkManager.DelListener(MessageID.MSG_NOTIFY_INTO_SCENE, OnAck_NotifyIntoScene);
        NetworkManager.DelListener(MessageID.MSG_ENTER_SCENE_ACK,   OnAck_EnterScene);
    }

    private void OnAck_CreateRole(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckCreateRole ack = Serializer.Deserialize<AckCreateRole>(ms);
        DataDBSRole.Insert(ack.Player.Id, ack.Player);

        GTEventCenter.FireEvent(GTEventID.TYPE_CREATEROLE_CALLBACK);
    }

    private void OnAck_EnterGame(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckEnterGame ack = Serializer.Deserialize<AckEnterGame>(ms);
        GTLauncher.CurPlayerID = ack.Player.Id;
        GTEventCenter.FireEvent(GTEventID.TYPE_ENTERGAME_CALLBACK);
        GTLauncher.Instance.LoadScene(GTLauncher.LAST_CITY_ID);
    }

    private void OnAck_GetServerList(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ms.Seek(28, SeekOrigin.Begin);
        ClientServerListAck ack = Serializer.Deserialize<ClientServerListAck>(ms);
    }

    private void OnAck_RoleList(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        RoleListAck ack = Serializer.Deserialize<RoleListAck>(ms);
        GTEventCenter.FireEvent(GTEventID.TYPE_LOGINGAME_CALLBACK);

        if(ack.RoleNode.Count<=0)
        {
            GTLauncher.Instance.LoadScene(GTSceneKey.SCENE_Role);
        }
        else
        {
            RoleLoginReq req = new RoleLoginReq();
            req.AccountID = LoginService.Instance.m_CurAccountID;
            req.RoleID = ack.RoleNode[0].ID;
            req.LoginCode = LoginService.Instance.m_LoginCode;
            NetworkManager.Instance.Send(MessageID.MSG_ROLE_LOGIN_REQ, req, 0, 0);
        }
    }

    private void OnAck_AccountLogin(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ms.Seek(28, SeekOrigin.Begin);
        AccountLoginAck ack = Serializer.Deserialize<AccountLoginAck>(ms);
        
        LoginService.Instance.m_CurServerID = ack.LastSvrID;
        LoginService.Instance.m_CurAccountID = ack.AccountID;
        LoginService.Instance.m_CurServerName = ack.LastSvrName;

        GTEventCenter.FireEvent(GTEventID.TYPE_ACCLOGIN_CALLBACK);
    }

    private void OnAck_Register(MessageRecv obj )
    {
        GTEventCenter.FireEvent(GTEventID.TYPE_REGISTER_CALLBACK);
    }

    private void OnAck_SelectServer(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ms.Seek(28, SeekOrigin.Begin);
        SelectServerAck ack = Serializer.Deserialize<SelectServerAck>(ms);
        NetworkManager.Instance.Close();
        NetworkManager.Instance.Start(ack.ServerAddr, ack.ServerPort);
        //等待连接游戏服务器
        //

        NetworkManager.Instance.WatiConnctOK();

        //连接上服务器之后，请求角色列表
        RoleListReq Req = new RoleListReq();
        Req.AccountID = LoginService.Instance.m_CurAccountID;
        Req.LoginCode = ack.LoginCode;
        NetworkManager.Instance.Send<RoleListReq>(MessageID.MSG_ROLE_LIST_REQ, Req, 0, 0);
    }


    private void OnAck_NotifyIntoScene(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ms.Seek(28, SeekOrigin.Begin);
        NotifyIntoScene ack = Serializer.Deserialize<NotifyIntoScene>(ms);

        EnterSceneReq req = new EnterSceneReq();
        req.RoleID = ack.RoleID;
        NetworkManager.Instance.Send<EnterSceneReq>(MessageID.MSG_ENTER_SCENE_REQ, req, (UInt64)ack.CopyID, (UInt32)ack.ServerID);
    }

    private void OnAck_EnterScene(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ms.Seek(28, SeekOrigin.Begin);
        EnterSceneAck ack = Serializer.Deserialize<EnterSceneAck>(ms);

        //GTLauncher.CurPlayerID = ack.Player.Id;
        ///GTEventCenter.FireEvent(GTEventID.TYPE_ENTERGAME_CALLBACK);
        //GTLauncher.Instance.LoadScene(GTLauncher.LAST_CITY_ID);
    }
    
    
}
