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
        NetworkManager.AddListener(MessageID.MSG_ACK_REGISTER,      OnAck_Register);
        NetworkManager.AddListener(MessageID.MSG_ACCOUNT_LOGIN_ACK, OnAck_Login);
        NetworkManager.AddListener(MessageID.MSG_ACK_LOGINSERVER,   OnAck_LoginGame);
        NetworkManager.AddListener(MessageID.MSG_ACK_GETSERVERLIST, OnAck_GetServerList);
        NetworkManager.AddListener(MessageID.MSG_SELECT_SERVER_ACK, OnAck_SelectServer);
        NetworkManager.AddListener(MessageID.MSG_ACK_ENTERGAME,     OnAck_EnterGame);
        NetworkManager.AddListener(MessageID.MSG_ACK_CREATEROLE,    OnAck_CreateRole);
    }

    public void DelListener()
    {
        NetworkManager.DelListener(MessageID.MSG_ACK_REGISTER, OnAck_Register);
        NetworkManager.DelListener(MessageID.MSG_ACK_LOGIN, OnAck_Login);
        NetworkManager.DelListener(MessageID.MSG_ACK_LOGINSERVER, OnAck_LoginGame);
        NetworkManager.DelListener(MessageID.MSG_ACK_GETSERVERLIST, OnAck_GetServerList);
        NetworkManager.DelListener(MessageID.MSG_ACK_ENTERGAME, OnAck_EnterGame);
        NetworkManager.DelListener(MessageID.MSG_ACK_CREATEROLE, OnAck_CreateRole);
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

    }

    private void OnAck_LoginGame(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckLoginGame ack = Serializer.Deserialize<AckLoginGame>(ms);

        GTEventCenter.FireEvent(GTEventID.TYPE_LOGINGAME_CALLBACK);
        GTLauncher.Instance.LoadScene(GTSceneKey.SCENE_Role);
    }

    private void OnAck_Login(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ms.Seek(28, SeekOrigin.Begin);
        AccountLoginAck ack = Serializer.Deserialize<AccountLoginAck>(ms);
        GTEventCenter.FireEvent(GTEventID.TYPE_LOGIN_CALLBACK);

        SelectServerReq Req = new SelectServerReq();
        Req.ServerID = 201;
        NetworkManager.Instance.Send<SelectServerReq>(MessageID.MSG_SELECT_SERVER_REQ, Req, 0, 0);

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

        RoleLoginReq Req = new RoleLoginReq();
        Req.AccountID = 1;
        Req.RoleID = 100000001;
        Req.LoginCode = 1111111;
        NetworkManager.Instance.Send<RoleLoginReq>(MessageID.MSG_ROLE_LOGIN_REQ, Req, 0, 0);
    }
    
}
