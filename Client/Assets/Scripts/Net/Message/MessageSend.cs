using UnityEngine;
using System.Collections;

public class MessageSend
{
    public NetworkClient Client { get; private set; }
    public byte[]        Bytes  { get; private set; }

    public MessageSend(NetworkClient client, byte[] bytes)
    {
        this.Client = client;
        this.Bytes  = bytes;
    }
}
