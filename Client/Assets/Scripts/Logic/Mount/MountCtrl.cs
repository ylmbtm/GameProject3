using UnityEngine;
using System.Collections;
using System;
using Protocol;
using ProtoBuf;

public class MountCtrl : ICtrl
{
    public void AddListener()
    {
        NetworkManager.AddListener(MessageID.MSG_ACK_SETMOUNT, OnAck_SetMount);
    }

    public void DelListener()
    {
        NetworkManager.DelListener(MessageID.MSG_ACK_SETMOUNT, OnAck_SetMount);
    }

    private void OnAck_SetMount(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckSetMount ack = Serializer.Deserialize<AckSetMount>(ms);

        XCharacter role = RoleModule.Instance.GetCurPlayer();
        role.Mount = ack.MountID;
        DataDBSRole.Update(role.Id, role);
        GTEventCenter.FireEvent(GTEventID.TYPE_DRESS_MOUNT);
    }

}
