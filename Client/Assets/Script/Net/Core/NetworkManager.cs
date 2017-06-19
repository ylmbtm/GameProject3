using UnityEngine;
using System; 
using System.Collections;
using System.Collections.Generic;
using System.Linq;

public class NetworkManager: GTSingleton<NetworkManager>
{
    private Dictionary<int, NetworkClient> m_MapCliConnection = new Dictionary<int, NetworkClient>();
    private Dictionary<MessageID, NetworkHandler> mMessageDispatchs = new Dictionary<MessageID, NetworkHandler>();

    public void Connect(int connid, string ip, int port)
    {
        if (connid <= 0)
        {
            return;
        }

        NetworkClient tempClient = null;
        m_MapCliConnection.TryGetValue(connid, out tempClient);
        if (tempClient != null)
        {
            tempClient->ConnectToServer(ip, port);
        }
        else
        {
            tempClient = new NetworkClient(connid);
            tempClient.ConnectToServer(ip, port);
        }
    }


    NetworkClient GetConnectionByID(int connID)
    {
        NetworkClient tempClient = null;
        m_MapCliConnection.TryGetValue(connID, out tempClient);
        return tempClient;
    }


    public void CloseConntion(int connID)
    {
        NetworkClient tempClient = null;
        m_MapCliConnection.TryGetValue(connID, out tempClient);
        if (tempClient != null)
        {
            tempClient->Close();
        }

        return;
    }

    public void CloseAll()
    {
        foreach (KeyValuePair<int, NetworkClient> kvp in m_MapCliConnection)
        {
            if (kvp.Value != null)
            {
                kvp.Value.Close();
            }
        }

        return;
    }

    void OnDestroy()
    {
        
    }

    void Update()
    {
        foreach (KeyValuePair<int, NetworkClient> kvp in m_MapCliConnection)
        {
            if (kvp.Value != null)
            {
                kvp.Value.Render();
            }
        }
    }
}
