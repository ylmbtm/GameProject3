using UnityEngine;
using System.Collections;
using Protocol;

public class MessageCallbackData
{
    public MessageID       ID;
    public NetworkCallback Handler;
    public object          Target;
}
