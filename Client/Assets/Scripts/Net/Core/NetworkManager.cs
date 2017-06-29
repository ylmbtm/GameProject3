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
    private NetworkClient                                mClient;
    private bool                                         mAlone = false;
    private static Dictionary<MessageID, NetworkHandler> mMessageDispatchs = new Dictionary<MessageID, NetworkHandler>();
    private static List<MessageRecv>                     mRecvPackets = new List<MessageRecv>();
    private static List<MessageSend>                     mSendPackets = new List<MessageSend>();
    private MessageCenter                                mMessageCenter  = new MessageCenter();

    public void Start(string ip, int port)
    {
        if (!mAlone)
        {
            Connect(ip, port);
        }
        mMessageCenter.AddHandlers();
    }

    public void Execute()
    {
        lock (mRecvPackets)
        {
            if (mRecvPackets.Count > 0)
            {
                for (int i = 0; i < mRecvPackets.Count; i++)
                {
                    DispatchPacket(mRecvPackets[i]);
                }
                mRecvPackets.Clear();
            }
        }
        lock (mSendPackets)
        {
            if (mSendPackets.Count > 0)
            {
                for (int i = 0; i < mSendPackets.Count; i++)
                {
                    Send(mSendPackets[i]);
                }
                mSendPackets.Clear();
            }
        }
    }

    public void Connect(string ip, int port)
    {
        mClient = new NetworkClient(ip, port);
        mClient.Connect();
    }

    public void Send<T>(MessageID messageID, T obj, UInt64 u64TargetID, UInt32 dwUserData)
    {
        if(mAlone)
        {
            byte[] bytes = null;
            Pack(messageID, obj, u64TargetID, dwUserData, ref bytes);
            Recv(mClient, bytes);
        }
        else
        {
            Send(mClient, messageID, obj, u64TargetID, dwUserData);
        }
    }

    public void Close()
    {
        if (mClient != null)
        {
            mClient.Close();
            mClient = null;
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
        lock (mRecvPackets)
        {
            mRecvPackets.Add(evet);
        }
    }

    public static void AddListener(MessageID id, NetworkHandler handle)
    {
        NetworkHandler h = null;
        mMessageDispatchs.TryGetValue(id, out h);
        if (h == null)
        {
            mMessageDispatchs.Add(id, handle);
        }
        else
        {
            mMessageDispatchs[id] += handle;
        }
    }

    public static void DelListener(MessageID id, NetworkHandler handle)
    {
        NetworkHandler h = null;
        mMessageDispatchs.TryGetValue(id, out h);
        if (h != null)
        {
            mMessageDispatchs[id] -= handle;
        }
    }

    static void DispatchPacket(MessageRecv recv)
    {
        NetworkHandler handle = null;
        mMessageDispatchs.TryGetValue(recv.MsgID, out handle);
        if (handle != null)
        {
            handle.Invoke(recv);
        }
    }

    public IEnumerator WatiConnctOK()  
    {
        while (!mClient.IsConnectOK())
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
        mSendPackets.Add(tab);
    }

    static public void WriteUint32(UInt32 v)
    {
        for (int i = 0; i < 4; i++)
        {
            m_PacketHeader[m_nWritePos++] = (Byte)(v >> i * 8 & 0xff);
        }
    }

    static public void WriteUint64(UInt64 v)
    {
        byte[] getdata = BitConverter.GetBytes(v);
        for (int i = 0; i < getdata.Length; i++)
        {
            m_PacketHeader[m_nWritePos++] = getdata[i];
        }
    }


    static public int m_nWritePos = 0;
    static public byte[] m_PacketHeader = new byte[28];

    static public bool MakePacketHeader(MessageID messageID, UInt64 u64Target, UInt32 dwUserData)
    {
        m_nWritePos = 0;

        UInt32 CheckCode = 0x88;
        UInt32 dwMsgID = (UInt32)messageID;
        UInt32 dwSize = 3;
        UInt32 dwPacketNo = 0;	//生成序号 = wCommandID^dwSize+index(每个包自动增长索引); 还原序号 = pHeader->dwPacketNo - pHeader->wCommandID^pHeader->dwSize;

        WriteUint32(CheckCode);
        WriteUint32(dwMsgID);
        WriteUint32(dwSize);
        WriteUint32(dwPacketNo);
        WriteUint64(u64Target);
        WriteUint32(dwUserData);
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