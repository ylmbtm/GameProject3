using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Protocol;
using System;

public class MLLogin : GTSingleton<MLLogin>
{
    private List<ClientServerNode> m_ServerList = new List<ClientServerNode>();
    private XServer                m_CurrServer;

    public List<ClientServerNode> GetServerList()
    {
        return m_ServerList;
    }

    public void                   SetServerList(List<ClientServerNode> value)
    {
        m_ServerList = value;
    }

    public XServer                GetCurrServer()
    {
        return m_CurrServer;
    }

    public int                    GetCurrServerID()
    {
        return m_CurrServer == null ? 0 : m_CurrServer.ID;
    }       

    public void                   SetCurrServer(ClientServerNode server)
    {
        m_CurrServer = new XServer();
        m_CurrServer.ID = server.SvrID;
        m_CurrServer.Name = server.SvrName;
        m_CurrServer.State =(int) server.SvrState;
    }

    public string                 LastUsername
    {
        get { return GTGlobal.LocalData.Username; }
        set
        {
            if (string.IsNullOrEmpty(value))
            {
                return;
            }
            GTGlobal.LocalData.Username = value;
        }
    }

    public string                 LastPassword
    {
        get { return GTGlobal.LocalData.Password; }
        set
        {
            if (string.IsNullOrEmpty(value))
            {
                return;
            }
            GTGlobal.LocalData.Password = value;
        }
    }

    public UInt64                 LastAccountID
    {
        get; set;
    }
}
