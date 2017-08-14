using UnityEngine;
using System.Collections;
using System.Net.Sockets;
using System.Net;
using System;
using System.Threading;
using Protocol;

public class NetworkClient
{
    private TcpClient       m_Tcp;
    private string          m_IP;
    private int             m_Port;
    private bool            m_IsConnect        = false;
    private Thread          m_RecvThread;
    private int             m_PacketHeaderSize = 28;
    private int             m_RecvLen          = 0;
    private int             m_DataLen          = 0;
    private byte[]          m_RecvBuffer       = new byte[8192];
    private byte[]          m_DataBuffer       = new byte[16384];
    private EventWaitHandle m_AllDone;

    public  Callback        OnConnectSuccess;

    public NetworkClient(string ip, int port)
    {
        m_IP     = ip;
        m_Port   = port;
    }

    public void Connect()
    {
        m_Tcp = new TcpClient();
        m_Tcp.BeginConnect(IPAddress.Parse(m_IP), m_Port, OnConnect, m_Tcp);
        GTAsync.Instance.Run(() =>
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_NETWORK_CONNECT);
        });
    }

    public void Send(byte [] bytes)
    {
        if (m_IsConnect == false)
        {
            OnError(MessageRetCode.MRC_DISCONNECT);
            return;
        }
        try
        {
            m_Tcp.Client.Send(bytes);
        }
        catch (Exception ex)
        {
            Debug.LogError(ex.ToString());
        }
    }

    public void Close()
    {
        OnClose();
    }

    public bool IsConnectOK()
    {
        return m_IsConnect;
    }

    void OnConnect(IAsyncResult ar)
    {
        if (m_Tcp.Client == null)
        {
            OnError(MessageRetCode.MRC_DISCONNECT);
            return;
        }
        if (!m_Tcp.Client.Connected)
        {
            OnError(MessageRetCode.MRC_DISCONNECT);
            return;
        }
        m_IsConnect = true;
        m_AllDone = new EventWaitHandle(false, EventResetMode.AutoReset);
        m_RecvThread = new Thread(OnReceive);
        m_RecvThread.Start();
        GTAsync.Instance.Run(() =>
        {
            if (OnConnectSuccess != null)
            {
                OnConnectSuccess.Invoke();
            }
            OnConnectSuccess = null;
            GTEventCenter.FireEvent(GTEventID.TYPE_NETWORK_CONNECT_SUCCESS);
        });
    }

    void OnReceive()
    {
        while (true)
        {
            Thread.Sleep(20);
            if (m_Tcp == null || m_Tcp.Connected == false)
            {
                OnError(MessageRetCode.MRC_DISCONNECT);
                return;
            }
            NetworkStream stream = m_Tcp.GetStream();
            if (stream.CanRead)
            {
                stream.BeginRead(m_RecvBuffer, 0, m_RecvBuffer.Length, new AsyncCallback(OnAsyncRead), stream);
                m_AllDone.WaitOne();
            }
        }
    }

    bool MakeRealPacket()
    {
        if (m_DataLen < m_PacketHeaderSize)
        {
            return false;
        }
        Byte CheckCode = m_DataBuffer[0];
        int nPacketSize = BitConverter.ToUInt16(m_DataBuffer, 8);
        if (nPacketSize > m_DataLen)
        {
            //暂时这样处理
            return false;
        }
        byte[] realPacket = new byte[nPacketSize];
        Array.Copy(m_DataBuffer, 0, realPacket, 0, nPacketSize);
        Array.Copy(m_DataBuffer, nPacketSize, m_DataBuffer, 0, m_DataLen - nPacketSize);
        m_DataLen = m_DataLen - nPacketSize;
        NetworkManager.RecvC2C(this, realPacket);
        return true;
    }

    void OnAsyncRead(IAsyncResult ar)
    {
        NetworkStream stream = (NetworkStream)ar.AsyncState;
        m_RecvLen = stream.EndRead(ar);
        if (m_RecvLen <= 0)
        {
            return;
        }
        Array.Copy(m_RecvBuffer, 0, m_DataBuffer, m_DataLen, m_RecvLen);
        m_DataLen += m_RecvLen;
        m_RecvLen = 0;
        while(MakeRealPacket());
        m_AllDone.Set();
    }

    void OnError(MessageRetCode retCode)
    {
        GTAsync.Instance.Run(() =>
        {
            GTEventCenter.FireEvent(GTEventID.TYPE_NETWORK_CONNECT_FAIL, retCode);
        });
        OnClose();
    }

    void OnClose()
    {
        m_IsConnect = false;
        if (m_RecvThread != null)
        {
            m_RecvThread.Abort();
            m_RecvThread = null;
        }
        if (m_Tcp != null && m_Tcp.Connected)
        {
            m_Tcp.Close();
        }
        m_Tcp = null;
    }
}
