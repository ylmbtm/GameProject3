using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using Protocol;
using ProtoBuf;

public class StoreCtrl : ICtrl
{
    public void AddListener()
    {
        NetworkManager.AddListener(MessageID.MSG_ACK_BUY_STORE, OnAck_BuyStore);
    }

    public void DelListener()
    {
        NetworkManager.DelListener(MessageID.MSG_ACK_BUY_STORE, OnAck_BuyStore);
    }

    private void OnAck_BuyStore(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckBuyStore ack = Serializer.Deserialize<AckBuyStore>(ms);

        DStore db = ReadCfgStore.GetDataById(ack.StoreID);
        KStruct data = new KStruct(db.ItemID, db.ItemNum * ack.Num);
        GTItemHelper.ShowAwardTip(data);
        DataManager.Instance.AddNewItem(db.ItemID, db.ItemNum * ack.Num);
        DataManager.Instance.UseMoney(db.CostMoneyID, db.CostMoneyNum * ack.Num);

        GTEventCenter.FireEvent(GTEventID.TYPE_BUY_STORE);
    }
}
