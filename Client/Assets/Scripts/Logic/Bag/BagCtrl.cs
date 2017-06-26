using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using Protocol;
using ProtoBuf;

public class BagCtrl :  ICtrl
{
    public void AddListener()
    {
        NetworkManager.AddListener(MessageID.MSG_ACK_SORTBAG, OnAck_SortBag);
        NetworkManager.AddListener(MessageID.MSG_ACK_DRESS_EQUIP, OnAck_DressEquip);
        NetworkManager.AddListener(MessageID.MSG_ACK_UNLOAD_EQUIP, OnAck_UnloadEquip);
        NetworkManager.AddListener(MessageID.MSG_ACK_DRESS_GEM, OnAck_DressGem);
        NetworkManager.AddListener(MessageID.MSG_ACK_UNLOAD_GEM, OnAck_UnloadGem);
        NetworkManager.AddListener(MessageID.MSG_ACK_COMPOSE_CHIP, OnAck_ComposeChip);
        NetworkManager.AddListener(MessageID.MSG_ACK_USEITEM, OnAck_UseItem);
    }

    public void DelListener()
    {
        NetworkManager.DelListener(MessageID.MSG_ACK_SORTBAG, OnAck_SortBag);
        NetworkManager.DelListener(MessageID.MSG_ACK_DRESS_EQUIP, OnAck_DressEquip);
        NetworkManager.DelListener(MessageID.MSG_ACK_UNLOAD_EQUIP, OnAck_UnloadEquip);
        NetworkManager.DelListener(MessageID.MSG_ACK_DRESS_GEM, OnAck_DressGem);
        NetworkManager.DelListener(MessageID.MSG_ACK_UNLOAD_GEM, OnAck_UnloadGem);
        NetworkManager.DelListener(MessageID.MSG_ACK_COMPOSE_CHIP, OnAck_ComposeChip);
        NetworkManager.DelListener(MessageID.MSG_ACK_USEITEM, OnAck_UseItem);
    }

    private void OnAck_UseItem(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUseItem ack = Serializer.Deserialize<AckUseItem>(ms);

        int pos = ack.Pos;
        int num = ack.Num;
        int id  = ack.ID;

        XItem item = DataDBSBagItem.GetDataById(pos);
        DItem itemDB = ReadCfgItem.GetDataById(item.Id);
        switch (itemDB.ItemType)
        {
            case EItemType.DRUG:
                DataManager.Instance.AddNewItem(itemDB.Data2, itemDB.Data1 * num);
                DataManager.Instance.UseItemById(itemDB.Id, num);
                break;
            case EItemType.BOX:
                break;
        }

        GTEventCenter.FireEvent(GTEventID.TYPE_USE_ITEM, id, num);
    }

    private void OnAck_ComposeChip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckComposeChip ack = Serializer.Deserialize<AckComposeChip>(ms);

        int srcPos = ack.SrcPos;
        int num    = ack.Num;

        XItem item = DataDBSBagChip.Dict[srcPos];
        DItem itemDB = ReadCfgItem.GetDataById(item.Id);
        DataManager.Instance.AddNewItem(itemDB.Data2, num);
        DataManager.Instance.UseItemById(item.Id, itemDB.Data1 * num);

        KStruct data = new KStruct(itemDB.Data2, num);
        GTItemHelper.ShowAwardTip(data);

        GTEventCenter.FireEvent(GTEventID.TYPE_COMPOSE_CHIP, srcPos);
    }

    private void OnAck_UnloadGem(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUnloadGem ack = Serializer.Deserialize<AckUnloadGem>(ms);

        int newPos = ack.NewPos;
        int tarPos = ack.TarPos;

        XItem dressGem = DataDBSDressGem.GetDataById(tarPos);
        XItem bagGem = new XItem();
        bagGem.Instance = dressGem.Instance;
        bagGem.Pos = newPos;
        bagGem.PosType = (int)EPosType.BagGem;
        bagGem.Id = dressGem.Id;
        bagGem.Num = 1;
        DataDBSBagGem.Insert(newPos, bagGem);
        DataDBSDressGem.Delete(tarPos);

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_UNLOAD_GEM, newPos, tarPos);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_DressGem(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckDressGem ack = Serializer.Deserialize<AckDressGem>(ms);

        int srcPos = ack.SrcPos;
        int tarPos = ack.TarPos;
        int index  = ack.Index;

        XItem sourceGem = DataDBSBagGem.GetDataById(srcPos);
        DGem gemDB = ReadCfgGem.GetDataById(sourceGem.Id);
        int pos = GemModule.Instance.GetDressGemPosByIndexAndTargetPos(index, gemDB.Pos);
        XItem dressGem = DataDBSDressGem.GetDataById(pos);

        if (dressGem != null)
        {
            int sourceInstanceID = sourceGem.Instance;
            int sourceId = sourceGem.Id;

            sourceGem.Instance = dressGem.Instance;
            sourceGem.Id = dressGem.Id;

            dressGem.Instance = sourceInstanceID;
            dressGem.Id = sourceId;

            DataDBSDressGem.Update(dressGem.Pos, dressGem);
            DataDBSBagGem.Update(sourceGem.Pos, sourceGem);
        }
        else
        {
            dressGem = new XItem();
            dressGem.Instance = sourceGem.Instance;
            dressGem.Pos = pos;
            dressGem.PosType = (int)EPosType.RoleGem;
            dressGem.Id = sourceGem.Id;
            dressGem.Num = 1;
            DataDBSDressGem.Insert(pos, dressGem);
            DataDBSBagGem.Delete(sourceGem.Pos);
        }

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_DRESS_GEM, srcPos, tarPos);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_UnloadEquip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUnloadEquip ack = Serializer.Deserialize<AckUnloadEquip>(ms);

        int newPos = ack.NewPos;
        int tarPos = ack.TarPos;

        XItem dressEquip = DataDBSDressEquip.GetDataById(tarPos);
        XItem bagItem = new XItem();
        bagItem.Instance = dressEquip.Instance;
        bagItem.Pos = newPos;
        bagItem.PosType = (int)EPosType.BagItem;
        bagItem.Id = dressEquip.Id;
        bagItem.Num = 1;
        DataDBSBagItem.Insert(newPos, bagItem);
        DataDBSDressEquip.Delete(tarPos);

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_EQUIP);

        GTEventCenter.FireEvent(GTEventID.TYPE_UNLOAD_EQUIP, newPos, tarPos);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_DressEquip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckDressEquip ack = Serializer.Deserialize<AckDressEquip>(ms);
        int srcPos = ack.SrcPos;
        int tarPos = ack.TarPos;

        XItem bagEquip = DataDBSBagItem.GetDataById(srcPos);
        DEquip equipDB = ReadCfgEquip.GetDataById(bagEquip.Id);
        XItem dressEquip = DataDBSDressEquip.GetDataById(equipDB.Pos);
        if (dressEquip != null)
        {
            int bagInstance = bagEquip.Instance;
            int bagId = bagEquip.Id;

            bagEquip.Id = dressEquip.Id;
            bagEquip.Instance = dressEquip.Instance;
            bagEquip.PosType = (int)EPosType.BagItem;

            dressEquip.Instance = bagInstance;
            dressEquip.PosType = (int)EPosType.RoleEquip;
            dressEquip.Id = bagId;
            DataDBSDressEquip.Update(equipDB.Pos, dressEquip);
            DataDBSBagItem.Update(srcPos, bagEquip);
        }
        else
        {
            dressEquip = new XItem();
            dressEquip.Instance = bagEquip.Instance;
            dressEquip.Pos = equipDB.Pos;
            dressEquip.PosType = (int)EPosType.RoleEquip;
            dressEquip.Id = bagEquip.Id;
            dressEquip.Num = 1;
            DataDBSDressEquip.Insert(equipDB.Pos, dressEquip);
            DataDBSBagItem.Delete(srcPos);
        }

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_EQUIP);

        GTEventCenter.FireEvent(GTEventID.TYPE_DRESS_EQUIP, srcPos, tarPos);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_SortBag(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckSortBag ack = Serializer.Deserialize<AckSortBag>(ms);

        EBagType bagType = (EBagType)ack.BagType;
        Dictionary<int, XItem> items = DataManager.Instance.GetBagDataByBagType(bagType);
        List<XItem> list = new List<XItem>();
        list.AddRange(items.Values);
        items.Clear();

        switch (bagType)
        {
            case EBagType.ITEM:
                {
                    list.Sort(BagModule.Instance.CompareBagItem);
                    DataDBSBagItem.Clear(false);
                    for (int i = 0; i < list.Count; i++)
                    {
                        list[i].Pos = i + 1;
                        DataDBSBagItem.Insert(list[i].Pos, list[i]);
                    }
                }
                break;
            case EBagType.GEM:
                {
                    list.Sort(BagModule.Instance.CompareBagGem);
                    DataDBSBagGem.Clear(false);
                    for (int i = 0; i < list.Count; i++)
                    {
                        list[i].Pos = i + 1;
                        DataDBSBagGem.Insert(list[i].Pos, list[i]);
                    }
                }
                break;
            case EBagType.CHIP:
                {
                    list.Sort(BagModule.Instance.CompareBagChip);
                    DataDBSBagChip.Clear(false);
                    for (int i = 0; i < list.Count; i++)
                    {
                        list[i].Pos = i + 1;
                        DataDBSBagChip.Insert(list[i].Pos, list[i]);
                    }
                }
                break;
            case EBagType.FASHION:
                {
                    list.Sort(BagModule.Instance.CompareBagFashion);
                    DataDBSBagFashion.Clear(false);
                    for (int i = 0; i < list.Count; i++)
                    {
                        list[i].Pos = i + 1;
                        DataDBSBagFashion.Insert(list[i].Pos, list[i]);
                    }
                }
                break;
            case EBagType.RUNE:
                {
                    list.Sort(BagModule.Instance.CompareBagRune);
                    DataDBSBagRune.Clear(false);
                    for (int i = 0; i < list.Count; i++)
                    {
                        list[i].Pos = i + 1;
                        DataDBSBagRune.Insert(list[i].Pos, list[i]);
                    }
                }
                break;
        }

        GTEventCenter.FireEvent(GTEventID.TYPE_SORT_BAG);
    }
}
