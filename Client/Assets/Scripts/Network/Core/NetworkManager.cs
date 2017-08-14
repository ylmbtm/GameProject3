using UnityEngine;
using System.Collections;
using System;
using System.IO;
using System.Text;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Net.Sockets;
using System.Net;
using Protocol;
using ProtoBuf;

public class NetworkManager : GTSingleton<NetworkManager>
{
    private NetworkClient                                          m_Client;
    private static Dictionary<MessageID, MessageCallbackData>      m_MessageDispatchs = new Dictionary<MessageID, MessageCallbackData>();
    private static List<MessageRecv>                               m_RecvPackets      = new List<MessageRecv>();
    private static List<MessageSend>                               m_SendPackets      = new List<MessageSend>();
    private NetworkCtrl                                            m_NetworkCtrl      = new NetworkCtrl();
    private static int                                             m_nWritePos        = 0;
    private static byte[]                                          m_PacketHeader     = new byte[28];
    private string                                                 m_LoginIP;
    private int                                                    m_Port;

    public override void Init()
    {
        m_NetworkCtrl.AddHandlers();
    }

    public void SetIPAddress(string ip, int port)
    {
        m_LoginIP = ip;
        m_Port    = port;
    }

    public void Execute()
    {
        lock (m_RecvPackets)
        {
            if (m_RecvPackets.Count > 0)
            {
                for (int i = 0; i < m_RecvPackets.Count; i++)
                {
                    MessageRecv recv = m_RecvPackets[i];
                    DispatchPacket(recv);
                }
                m_RecvPackets.Clear();
            }
        }
        lock (m_SendPackets)
        {
            if (m_SendPackets.Count > 0)
            {
                for (int i = 0; i < m_SendPackets.Count; i++)
                {
                    Send(m_SendPackets[i]);
                }
                m_SendPackets.Clear();
            }
        }
    }

    public void ConnectLoginServer()
    {
        m_Client = new NetworkClient(m_LoginIP, m_Port);
        m_Client.Connect();
    }

    public void AddMessageCenterHandler()
    {
        SServer.Instance.AddListener();
    }

    public void DelMessageCenterHandler()
    {
        SServer.Instance.DelListener();
    }

    public void ConnectGameServer(string ip,int port, Callback onConnect)
    {
        m_Client = new NetworkClient(ip, port);
        m_Client.OnConnectSuccess = onConnect;
        m_Client.Connect();
    }

    public void ReConnect()
    {
        if (m_Client != null)
        {
            m_Client.Connect();
        }
    }

    public void Send<T>(MessageID messageID, T obj, UInt64 u64TargetID, UInt32 dwUserData)
    {
        if(GTGlobal.IsAlong)
        {
            byte[] bytes = null;
            Pack(messageID, obj, u64TargetID, dwUserData, ref bytes);
            RecvC2C(m_Client, bytes);
        }
        else
        {
            Send(m_Client, messageID, obj, u64TargetID, dwUserData);
        }
    }

    public void Send<T>(MessageID messageID, T obj)
    {
        Send<T>(messageID, obj, 0, 0);
    }

    public void Close()
    {
        if (m_Client != null)
        {
            m_Client.Close();
            m_Client = null;
        }   
    }

    public static void Recv(NetworkClient client, byte[] bytes)
    {
        MemoryStream ms = new MemoryStream(bytes);
        MessageRecv evet = new MessageRecv();
        evet.Data = bytes;
        evet.Client = client;
        evet.MsgID = (MessageID)BitConverter.ToInt32(bytes, 4);
        evet.UesrData = BitConverter.ToUInt32(bytes, 24);
        evet.TargetID = BitConverter.ToUInt64(bytes, 16);
        lock (m_RecvPackets)
        {
            m_RecvPackets.Add(evet);
        }
    }

    public static void RecvC2C(NetworkClient client, byte[] bytes)
    {
        byte[] data = new byte[bytes.Length - 28];
        for (int i = 0; i < data.Length; i++)
        {
            data[i] = bytes[i + 28];
        }
        MessageRecv evet = new MessageRecv();
        evet.Data = data;
        evet.Client = client;
        evet.MsgID = (MessageID)BitConverter.ToInt32(bytes, 4);
        evet.UesrData = BitConverter.ToUInt32(bytes, 24);
        evet.TargetID = BitConverter.ToUInt64(bytes, 16);
        lock (m_RecvPackets)
        {
            m_RecvPackets.Add(evet);
        }
    }

    public static void AddListener(MessageID id, NetworkCallback handle)
    {
        AddListener(id, handle, null);
    }

    public static void AddListener(MessageID id, NetworkCallback handle, object target)
    {
        MessageCallbackData d = null;
        m_MessageDispatchs.TryGetValue(id, out d);
        if (d == null)
        {
            d = new MessageCallbackData();
            m_MessageDispatchs[id] = d;
        }
        d.Handler = handle;
        d.Target = target;
        d.ID = id;
    }

    public static void DelListener(MessageID id, NetworkCallback handle)
    {
        m_MessageDispatchs[id] = null;
    }

    public static void DelListenerByTarget(object target)
    {
        List<MessageCallbackData> list = new List<MessageCallbackData>(m_MessageDispatchs.Values);
        for (int i = 0; i < list.Count; i++)
        {
            if(list[i].Target == target)
            {
                m_MessageDispatchs.Remove(list[i].ID);
            }
        }
    }

    static void DispatchPacket(MessageRecv recv)
    {
        MessageCallbackData d = null;
        m_MessageDispatchs.TryGetValue(recv.MsgID, out d);
        if (d != null)
        {
            d.Handler(recv);
        }
    }

    public IEnumerator WaitConnectOK()  
    {
        while (!m_Client.IsConnectOK())
        {
            yield return 0;
        }
        Debug.Log("connect is Ok!");  
    }  

    static void Send(MessageSend msg)
    {
        msg.Client.Send(msg.Bytes);
    }

    static void Send<T>(NetworkClient client, MessageID messageID, T obj, UInt64 u64TargetID, UInt32 dwUserData)
    {
        byte[] bytes = null;
        Pack<T>(messageID, obj, u64TargetID, dwUserData , ref bytes);
        MessageSend tab = new MessageSend(client, bytes);
        m_SendPackets.Add(tab);
    }

    static public void WriteUInt32(UInt32 v)
    {
        for (int i = 0; i < 4; i++)
        {
            m_PacketHeader[m_nWritePos++] = (Byte)(v >> i * 8 & 0xff);
        }
    }

    static public void WriteUInt64(UInt64 v)
    {
        byte[] getdata = BitConverter.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_PacketHeader[m_nWritePos++] = getdata[i];
        }
    }

    static public bool MakePacketHeader(MessageID messageID, UInt64 u64Target, UInt32 dwUserData)
    {
        m_nWritePos = 0;
        UInt32 CheckCode = 0x88;
        UInt32 dwMsgID = (UInt32)messageID;
        UInt32 dwSize = 3;
        UInt32 dwPacketNo = 0;	//生成序号 = wCommandID^dwSize+index(每个包自动增长索引); 还原序号 = pHeader->dwPacketNo - pHeader->wCommandID^pHeader->dwSize;
        WriteUInt32(CheckCode);
        WriteUInt32(dwMsgID);
        WriteUInt32(dwSize);
        WriteUInt32(dwPacketNo);
        WriteUInt64(u64Target);
        WriteUInt32(dwUserData);
        return true;
    }

    static void Pack<T>(MessageID messageID, T obj, UInt64 u64TargetID, UInt32 dwUserData, ref byte[] bytes)
    {
        MemoryStream byteMs = new MemoryStream();
        MakePacketHeader(messageID, u64TargetID, dwUserData);
        byteMs.Write(m_PacketHeader, 0, 28);
        Serializer.Serialize<T>(byteMs, obj);
        bytes = byteMs.ToArray();
        UInt32 nLen = (UInt32)bytes.Length;
        int nPos = 8;
        for (int i = 0; i < 4; i++)
        {
            bytes[nPos++] = (Byte)(nLen >> i * 8 & 0xff);
        }
    }
}