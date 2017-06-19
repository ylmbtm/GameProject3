using UnityEngine;
using System; 
using System.Collections.Generic;
using System.Net.Sockets;
using System.Net;

public enum ConnectState
{
    Not_Connect,
    Start_Connect,
    Raw_Connect,
    Succ_Connect
};


public class NetworkClient
{
    public string m_strIp = "";
	public UInt16 m_sPort = 0;

    private int m_ConnID = 0;

    // Socket客户端对象
    private Socket m_ClientSocket = null;
    public ConnectState m_ConnectState;

    public Int32 mTimeOut = 500;
    private int PacketHeaderSize = 18;

    public const int BUFFER_MAX = 8096;
    private int m_RecvLen = 0;
    private int m_DataLen = 0;

    private byte[] m_RecvBuffer = new byte[BUFFER_MAX];
    private byte[] m_DataBuffer = new byte[BUFFER_MAX];

    public NetworkClient(int ConnID)
    {
        m_ConnID = ConnID;
    }

    public Boolean InitConnection() { return true; }

    public Boolean CloseConnection() { return true; }

    //public Boolean ConnectToServer(string strIpAddr, UInt16 sPort)
    //{
    //    IPAddress ipAddress = IPAddress.Parse(strIpAddr);

    //    IPEndPoint ipEndpoint = new IPEndPoint(ipAddress, sPort);

    //    m_ClientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

    //    m_ClientSocket.SendTimeout = mTimeOut;

    //    try
    //    {
    //        // 这是一个异步的建立连接，当连接建立成功时调用connectCallback方法
    //        m_ClientSocket.BeginConnect(ipEndpoint, new AsyncCallback(ConnectCallBack), m_ClientSocket);
    //    }
    //    catch (System.Exception ex)
    //    {
    //        Debug.Print("Connect Exception: " + ex.Message);
    //    }

    //    SetConnectState(ConnectState.Start_Connect);

    //    return true;
    //}

    public Boolean ConnectToServer(string strIpAddr, UInt16 sPort)
    {
        IPAddress ipAddress = IPAddress.Parse(strIpAddr);

        IPEndPoint ipEndpoint = new IPEndPoint(ipAddress, sPort);

        m_ClientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

        m_ClientSocket.SendTimeout = mTimeOut;

        try
        {
            m_ClientSocket.Connect(ipEndpoint);
        }
        catch (System.Exception ex)
        {
            Debug.Log("Connect Exception: " + ex.Message);

            return false;
        }

         m_ClientSocket.Blocking = false;

        SetConnectState(ConnectState.Raw_Connect);

        return true;
    }

    public Boolean DisConnect()
    {
        try
        {
            if (m_ClientSocket != null)
            {
                m_ClientSocket.Shutdown(SocketShutdown.Both);
                m_ClientSocket.Disconnect(true);
                m_ClientSocket.Close(10);
                m_ClientSocket = null;
            }
        }
        catch (System.Exception ex)
        {
            Debug.Log("SocketObject Closed Exception: " + ex.Message);
        }

        finally
        {
            SetConnectState(ConnectState.Not_Connect);
        }

        return true;
    }

    public Boolean SendData(byte[] pData, int dwLen)
    {
        if ((m_ClientSocket == null) || (!m_ClientSocket.Connected))
        {
            return false;
        }

        try
        {
            m_ClientSocket.Send(pData, dwLen, SocketFlags.None);
        }
        catch (System.Exception ex)
        {
            Debug.Log("SocketObject SendMessage Exception: " + ex.Message);
        }

        return true;
    }

    public Boolean Render()
    {
        if (m_ClientSocket == null || !m_ClientSocket.Connected)
        {
            return false;
        }

        if (m_ConnectState == ConnectState.Not_Connect)
        {
            return false;
        }

        if (m_ConnectState == ConnectState.Start_Connect)
        {
            return false;
        }

        ReceiveData();
        
        while(ProcessData());
        
        return true;
    }

	//以下是内部的消息处理
    public Boolean OnCommandHandle(Command_ID wCommandID, UInt64 u64ConnID, ReadBufferHelper ReadHelper) 
    {
        Boolean bHandled = false;

        switch (wCommandID)
        {
            case Command_ID.CMD_CONNECT_NOTIFY:
                {
                    OnCmdConnectNotify(wCommandID, u64ConnID, ReadHelper);

                    bHandled = true;
                }
                break;
            case Command_ID.CMD_CHAR_PICK_CHAR_ACK:
                {
                    OnCmdPickCharAck(wCommandID, u64ConnID, ReadHelper);

                    bHandled = true;
                }
                break;

            default:
                {
                    foreach (MessageHandler h in m_MsghandlerList)
                    {
                        if (h.OnCommandHandle(wCommandID, u64ConnID, ReadHelper))
                        {
                            bHandled = true;
                            break;
                        }
                    }

                }
                break;
        }
       
        if (!bHandled)
        {
            Debug.Log("Message" +  wCommandID +" is not by handled!");
        }

        return true; 
    }

    //以下都是内部网络实现

    bool IsSocketValid()
    {
        return ((m_ClientSocket != null) && (m_ClientSocket.Connected == true));
    }

    void SetConnectState(ConnectState conState)
    {
        m_ConnectState = conState;
    }

    void ConnectCallBack(IAsyncResult asyncConnect)
    {
        Socket ClientSocket = (Socket)asyncConnect.AsyncState;
        try
        {
            ClientSocket.EndConnect(asyncConnect);

            // 与socket建立连接成功，开启线程接受服务端数据
            SetConnectState(ConnectState.Raw_Connect);

            Debug.Log("SocketObject Connect Success.");
        }
        catch (Exception e)
        {
            Debug.Log("SocketObject Connect Failed: " + e.ToString());
        }
    }

    public bool IsConnectSuccessed()
    {
        return (m_ConnectState == ConnectState.Succ_Connect);
    }

   bool ProcessData()
    {
        if (m_DataLen < PacketHeaderSize)
        {
            if (m_RecvLen <= 0)
            {
                return false;
            }

            Array.Copy(m_RecvBuffer, 0, m_DataBuffer, m_DataLen, m_RecvLen);

            m_DataLen += m_RecvLen;

            m_RecvLen = 0;
        }

       if (m_DataLen < PacketHeaderSize)
       {
           return false;
       }

       Byte CheckCode = m_DataBuffer[0];

       int nPacketSize = BitConverter.ToUInt16(m_DataBuffer, 1);

       if(nPacketSize <= m_DataLen)
       {
           //说明一个包的数据是足够了
           Array.Copy(m_DataBuffer, 0, m_ReadHelper.GetData(), 0, nPacketSize);

           Array.Copy(m_DataBuffer, nPacketSize, m_DataBuffer, 0, m_DataLen - nPacketSize);

           m_DataLen = m_DataLen - nPacketSize;
       }
       else
       {
           return false;
       }

       OnCommandHandle(m_ConnID, (Command_ID)m_ReadHelper.m_CommandHeader.wCommandID, 0, m_ReadHelper);

       return true;
    }

   bool ReceiveData()
    {
        if (!IsSocketValid())
        {
            Debug.Log("invalid socket!");
            return false;
        }

        try
        {
            m_RecvLen = m_ClientSocket.Receive(m_RecvBuffer, BUFFER_MAX, 0);
        }
        catch (SocketException err)
        {
            if (err.ErrorCode == 10054 || err.ErrorCode == 10053)
            {
                Debug.Log(string.Format("NetworkInterface::recv(): disable connect!"));
                if (m_ClientSocket != null && m_ClientSocket.Connected)
                {
                    m_ClientSocket.Close();
                }

                m_ClientSocket = null;
            }
            else
            {
                Debug.Log(string.Format("NetworkInterface::recv(): socket error(" + err.ErrorCode + ")!"));
            }

            return false;
        }

        if (m_RecvLen <= 0)
        {
            Debug.Log(string.Format("NetworkInterface::recv(): disable connect!"));
            if (m_ClientSocket != null && m_ClientSocket.Connected)
            {
                m_ClientSocket.Close();
            }

            m_ClientSocket = null;

            m_RecvLen = 0;

            return false;
        }

        return true;
    }
}
