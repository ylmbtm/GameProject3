using UnityEngine;
using System.Collections;
using Protocol;
using System;

public class MessageRecv
{
    public NetworkClient Client;
    public MessageID     MsgID;
    public UInt64        TargetID;
    public UInt32        UesrData;
    public byte[]        Data { get; set; }
}
