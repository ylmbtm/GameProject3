using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using ProtoBuf;
using Protocol;

public class EquipCtrl :  ICtrl
{
    public void AddListener()
    {
        NetworkManager.AddListener(MessageID.MSG_ACK_STRENGTHEN_EQUIP, OnAck_StrengthEquip);
        NetworkManager.AddListener(MessageID.MSG_ACK_ADVANCE_EQUIP, OnAck_AdvanceEquip);
        NetworkManager.AddListener(MessageID.MSG_ACK_UPSTAR_EQUIP, OnAck_UpStarEquip);
    }

    public void DelListener()
    {
        NetworkManager.DelListener(MessageID.MSG_ACK_STRENGTHEN_EQUIP, OnAck_StrengthEquip);
        NetworkManager.DelListener(MessageID.MSG_ACK_ADVANCE_EQUIP, OnAck_AdvanceEquip);
        NetworkManager.DelListener(MessageID.MSG_ACK_UPSTAR_EQUIP, OnAck_UpStarEquip);
    }

    private void OnAck_UpStarEquip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUpStarEquip ack = Serializer.Deserialize<AckUpStarEquip>(ms);


        XEquip equip = ack.TarEquip;
        DEquip cfg    = ReadCfgEquip.GetDataById(equip.Id);
        int      starID = cfg.Quality * 1000 + equip.StarLevel + 1;

        DEquipStar db = ReadCfgEquipStar.GetDataById(starID);
        DataManager.Instance.UseMoney(db.CostMoneyId, db.CostMoneyNum);
        DataManager.Instance.UseItemById(db.CostItemId, db.CostItemNum);
        equip.StarLevel++;
        DataDBSEquip.Update(equip.Instance, equip);

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_UPSTAR_EQUIP);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_AdvanceEquip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckAdvanceEquip ack = Serializer.Deserialize<AckAdvanceEquip>(ms);

        XEquip      equip     = ack.TarEquip;
        List<XItem> list      = ack.UseItems;
        DEquip      equipDB   = ReadCfgEquip.GetDataById(equip.Id);
        int         advanceID = equipDB.Quality * 1000 + equip.AdvanceLevel + 1;

        DEquipAdvanceCost db = ReadCfgEquipAdvanceCost.GetDataById(advanceID);
        DataManager.Instance.UseMoney(db.CostMoneyId, db.CostMoneyNum);
        DataManager.Instance.UseItemById(db.CostItemId, db.CostItemNum);
        if (list != null)
        {
            for (int i = 0; i < list.Count; i++)
            {
                DataManager.Instance.DelBagEquip(list[i].Pos);
            }
        }
        equip.AdvanceLevel++;
        DataDBSEquip.Update(equip.Instance, equip);

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_ADVANCE_EQUIP);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_StrengthEquip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckStrengthEquip ack = Serializer.Deserialize<AckStrengthEquip>(ms);

        XEquip      equip = ack.TarEquip;
        List<XItem> list  = ack.UseItems;
        DEquip    cfg   = ReadCfgEquip.GetDataById(equip.Id);
        int strengthenLevel = equip.StrengthenLevel;
        int strID           = cfg.Quality * 1000 + strengthenLevel + 1;
        int addExpNum       = EquipModule.Instance.GetStrengthenExpByMatList(list);
 
        DEquipStrengLevel db = ReadCfgEquipStrengLevel.GetDataById(strID);
        int expNum = db.RequireExp - equip.StrengthenExp;
        while (addExpNum >= expNum)
        {
            strengthenLevel++;
            db = ReadCfgEquipStrengLevel.GetDataById(cfg.Quality * 1000 + strengthenLevel + 1);
            expNum += db.RequireExp;
        }
        equip.StrengthenLevel = strengthenLevel;
        equip.StrengthenExp = addExpNum + db.RequireExp - expNum;

        DataDBSEquip.Update(equip.Instance, equip);
        DataManager.Instance.UseMoney(DEquip.EQUIP_STRENGTHEN_MONEY_ID_1, expNum);
        for (int i = 0; i < list.Count; i++)
        {
            XItem item = list[i];
            if ((EPosType)item.PosType == EPosType.RoleMoney)
            {
                DataManager.Instance.UseMoney(item.Id, item.Num);
            }
            else
            {
                DataManager.Instance.DelBagEquip(item.Pos);
            }
        }

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_STRENGTHEN_EQUIP);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

}