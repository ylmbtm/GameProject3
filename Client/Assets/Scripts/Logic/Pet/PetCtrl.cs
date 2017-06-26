using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using Protocol;
using ProtoBuf;

public class PetCtrl : ICtrl
{
    public void AddListener()
    {
        NetworkManager.AddListener(MessageID.MSG_ACK_BATTLE_PET,  OnAck_BattlePet);
        NetworkManager.AddListener(MessageID.MSG_ACK_UNLOAD_PET,  OnAck_UnloadPet);
        NetworkManager.AddListener(MessageID.MSG_ACK_UPGRADE_PET, OnAck_UpgradePet);
    }

    public void DelListener()
    {
        NetworkManager.DelListener(MessageID.MSG_ACK_BATTLE_PET, OnAck_BattlePet);
        NetworkManager.DelListener(MessageID.MSG_ACK_UNLOAD_PET, OnAck_UnloadPet);
        NetworkManager.DelListener(MessageID.MSG_ACK_UPGRADE_PET, OnAck_UpgradePet);
    }

    private void OnAck_UpgradePet(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUpgradePet ack   = Serializer.Deserialize<AckUpgradePet>(ms);
        int           id    = ack.ID;
        List<XItem>   items = ack.UseItems;

        DActor db = ReadCfgActor.GetDataById(id);
        XPet data = DataDBSPet.GetDataById(id);
        if (data == null)
        {
            data = DataFactory.CreatePet(id);
        }

        int exp = PetModule.Instance.GetExpByItemList(items);
        DPetLevel levelDB = ReadCfgPetLevel.GetDataById(db.Quality * 1000 + data.Level);
        int addExp = exp + data.CurExp;
        while (addExp > levelDB.Exp)
        {
            addExp -= levelDB.Exp;
            data.Level++;
            levelDB = ReadCfgPetLevel.GetDataById(db.Quality * 1000 + data.Level);
        }
        data.CurExp = addExp;
        DataDBSPet.Update(id, data);
        for (int i = 0; i < items.Count; i++)
        {
            DataManager.Instance.UseItemById(items[i].Id, 1);
        }

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_UPGRADE_PET);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_UnloadPet(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUnloadPet ack = Serializer.Deserialize<AckUnloadPet>(ms);

        XCharacter role = RoleModule.Instance.GetCurPlayer();
        role.PetID = 0;
        DataDBSRole.Update(role.Id, role);

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_UNLOAD_PET);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_BattlePet(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckBattlePet ack = Serializer.Deserialize<AckBattlePet>(ms);

        XCharacter role = RoleModule.Instance.GetCurPlayer();
        role.PetID = ack.ID;
        DataDBSRole.Update(role.Id, role);

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_BATTLE_PET);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }
}
