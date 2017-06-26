using UnityEngine;
using System.Collections;
using System.Net.Sockets;
using System.Net;
using System;
using System.Threading;
using Protocol;

public class NetworkClient
{
    private TcpClient       mTcp;
    private string          mIP;
    private int             mPort;
    private bool            mIsConnect = false;
    private Thread          mRecvThread;

    private int PacketHeaderSize = 28;

    private int m_RecvLen = 0;
    private int m_DataLen = 0;

    private byte[] m_RecvBuffer = new byte[8192];
    private byte[] m_DataBuffer = new byte[8192];

    private EventWaitHandle mAllDone;

    public NetworkClient(string ip, int port)
    {
        mTcp    = new TcpClient();
        mIP     = ip;
        mPort   = port;
    }

    public void Connect()
    {
        mTcp.BeginConnect(IPAddress.Parse(mIP), mPort, OnConnect, mTcp);
    }

    public void Send(byte [] bytes)
    {
        if(mIsConnect==false)
        {
            OnError(MessageRetCode.TYPE_DISCONNECT);
            return;
        }
        try
        {
            mTcp.Client.Send(bytes);
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

    void OnConnect(IAsyncResult ar)
    {
        if (mTcp.Client == null)
        {
            OnError(MessageRetCode.TYPE_DISCONNECT);
            return;
        }
        if (!mTcp.Client.Connected)
        {
            OnError(MessageRetCode.TYPE_DISCONNECT);
            return;
        }
        mIsConnect = true;
        mAllDone = new EventWaitHandle(false, EventResetMode.AutoReset);
        mRecvThread = new Thread(OnReceive);
        mRecvThread.Start();
    }

    void OnReceive()
    {
        while (true)
        {
            Thread.Sleep(20);
            if (mTcp == null || mTcp.Connected == false)
            {
                OnError(MessageRetCode.TYPE_DISCONNECT);
                return;
            }
            NetworkStream stream = mTcp.GetStream();
            if (stream.CanRead)
            {
                stream.BeginRead(m_RecvBuffer, 0, m_RecvBuffer.Length, new AsyncCallback(OnAsyncRead), stream);
                mAllDone.WaitOne();
            }
        }
    }

    void OnAsyncRead(IAsyncResult ar)
    {
        NetworkStream stream = (NetworkStream)ar.AsyncState;
        m_RecvLen = stream.EndRead(ar);

        if (m_DataLen < PacketHeaderSize)
        {
            if (m_RecvLen <= 0)
            {
                return ;
            }

            Array.Copy(m_RecvBuffer, 0, m_DataBuffer, m_DataLen, m_RecvLen);

            m_DataLen += m_RecvLen;

            m_RecvLen = 0;
        }

        if (m_DataLen < PacketHeaderSize)
        {
            return ;
        }


        Byte CheckCode = m_DataBuffer[0];

        int nPacketSize = BitConverter.ToUInt16(m_DataBuffer, 8);

        if (nPacketSize > m_DataLen)
        {
            //暂时这样处理
            return ;
        }

        byte[] realPacket = new byte[nPacketSize];

        Array.Copy(m_DataBuffer, 0, realPacket, 0, nPacketSize);

        Array.Copy(m_DataBuffer, nPacketSize, m_DataBuffer, 0, m_DataLen - nPacketSize);

        m_DataLen = m_DataLen - nPacketSize;

        NetworkManager.Recv(this, realPacket);

        mAllDone.Set();
    }

    void OnError(MessageRetCode retCode)
    {
        if(retCode == MessageRetCode.TYPE_OK)
        {
            return;
        }
        Debug.LogError(retCode.ToString());
        OnClose();
    }

    void OnClose()
    {
        mIsConnect = false;
        if (mRecvThread != null)
        {
            mRecvThread.Abort();
            mRecvThread = null;
        }
        if (mTcp != null && mTcp.Connected)
        {
            mTcp.Close();
        }
        mTcp = null;
    }

}
