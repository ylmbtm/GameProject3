using UnityEngine;
using System.Collections;
using Protocol;
using ProtoBuf;
using LitJson;
using System;
using System.Collections.Generic;
using MAP;
using CFG;

public class SServerCenter
{
    public void AddHandlers()
    {
        NetworkManager.AddListener(MessageID.MSG_ACCOUNT_LOGIN_REQ,    OnReq_Login,           this);
        NetworkManager.AddListener(MessageID.MSG_SELECT_SERVER_REQ,    OnReq_SelectServer,    this);
        NetworkManager.AddListener(MessageID.MSG_SERVER_LIST_REQ,      OnReq_GetServerList,   this);
        NetworkManager.AddListener(MessageID.MSG_ROLE_LOGIN_REQ,       OnReq_RoleLogin,       this);
        NetworkManager.AddListener(MessageID.MSG_ACCOUNT_REG_REQ,      OnReq_Register,        this);
        NetworkManager.AddListener(MessageID.MSG_ROLE_CREATE_REQ,      OnReq_CreateRole,      this);
        NetworkManager.AddListener(MessageID.MSG_ENTER_SCENE_REQ,      OnReq_EnterScene,      this);

        NetworkManager.AddListener(MessageID.MSG_MAIN_COPY_REQ,        OnReq_MainCopy,        this);
        NetworkManager.AddListener(MessageID.MSG_OBJECT_ACTION_REQ,    OnReq_ObjectAction,    this);

        NetworkManager.AddListener(MessageID.MSG_REQ_SORTBAG,          OnReq_SortBag,         this);
        NetworkManager.AddListener(MessageID.MSG_REQ_USEITEM,          OnReq_UseItem,         this);
        NetworkManager.AddListener(MessageID.MSG_REQ_DRESS_EQUIP,      OnReq_DressEquip,      this);
        NetworkManager.AddListener(MessageID.MSG_REQ_UNLOAD_EQUIP,     OnReq_UnloadEquip,     this);
        NetworkManager.AddListener(MessageID.MSG_REQ_DRESS_GEM,        OnReq_DressGem,        this);
        NetworkManager.AddListener(MessageID.MSG_REQ_UNLOAD_GEM,       OnReq_UnloadGem,       this);

        NetworkManager.AddListener(MessageID.MSG_REQ_COMPOSE_CHIP,     OnReq_ComposeChip,     this);
        NetworkManager.AddListener(MessageID.MSG_REQ_STRENGTHEN_EQUIP, OnReq_StrengthEquip,   this);
        NetworkManager.AddListener(MessageID.MSG_REQ_ADVANCE_EQUIP,    OnReq_AdvanceEquip,    this);
        NetworkManager.AddListener(MessageID.MSG_REQ_UPSTAR_EQUIP,     OnReq_UpStarEquip,     this);
        NetworkManager.AddListener(MessageID.MSG_REQ_STRENGTHEN_GEM,   OnReq_StrengthGem,     this);


        NetworkManager.AddListener(MessageID.MSG_REQ_SETMOUNT,         OnReq_SetMount,        this);

        NetworkManager.AddListener(MessageID.MSG_REQ_CHARGE_RELICS,    OnReq_ChargeRelics,    this);
        NetworkManager.AddListener(MessageID.MSG_REQ_UPGRADE_RELICS,   OnReq_UpgradeRelics,   this);
        NetworkManager.AddListener(MessageID.MSG_REQ_BATTLE_RELICS,    OnReq_BattleRelics,    this);
        NetworkManager.AddListener(MessageID.MSG_REQ_UNLOAD_RELICS,    OnReq_UnloadRelics,    this);

        NetworkManager.AddListener(MessageID.MSG_REQ_BUY_STORE,        OnReq_BuyStore,        this);

        NetworkManager.AddListener(MessageID.MSG_REQ_UPGRADE_PET,      OnReq_UpgradePet,      this);
        NetworkManager.AddListener(MessageID.MSG_REQ_BATTLE_PET,       OnReq_BattlePet,       this);
        NetworkManager.AddListener(MessageID.MSG_REQ_UNLOAD_PET,       OnReq_UnloadPet,       this);

        NetworkManager.AddListener(MessageID.MSG_REQ_CHANGE_PARTNER,   OnReq_ChangePartner,   this);
        NetworkManager.AddListener(MessageID.MSG_REQ_ADVANVE_PARTNER,  OnReq_AdvancePartner,  this);
        NetworkManager.AddListener(MessageID.MSG_REQ_UPGRADE_PARTNER,  OnReq_UpgradePartner,  this);

        NetworkManager.AddListener(MessageID.MSG_REQ_SUBMIT_TASK,      OnReq_SubmitTask,      this);
    }

    public void DelHandlers()
    {
        NetworkManager.DelListenerByTarget(this);
    }

    private void OnReq_Login(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AccountLoginReq req = Serializer.Deserialize<AccountLoginReq>(ms);
        AccountLoginAck ack = new AccountLoginAck();
        NetworkManager.Instance.Send(MessageID.MSG_ACCOUNT_LOGIN_ACK, ack, 0, 0);
    }

    private void OnReq_SelectServer(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        SelectServerReq req = Serializer.Deserialize<SelectServerReq>(ms);
        SelectServerAck ack = new SelectServerAck();
        GTDataManager.Instance.LoadCommonData();
        NetworkManager.Instance.Send(MessageID.MSG_SELECT_SERVER_ACK, ack, 0, 0);
    }

    private void OnReq_GetServerList(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ClientServerListReq req = Serializer.Deserialize<ClientServerListReq>(ms);
        ClientServerListAck ack = new ClientServerListAck();
        NetworkManager.Instance.Send(MessageID.MSG_SERVER_LIST_ACK, ack, 0, 0);
    }

    private void OnReq_Register(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AccountRegReq req = Serializer.Deserialize<AccountRegReq>(ms);
        AccountRegAck ack = new AccountRegAck();
        NetworkManager.Instance.Send(MessageID.MSG_ACCOUNT_REG_ACK, ack, 0, 0);
    }

    private void OnReq_CreateRole(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        RoleCreateReq req = Serializer.Deserialize<RoleCreateReq>(ms);

        RoleCreateAck ack = new RoleCreateAck();
        ack.RoleID = GTGUID.NewGUID();
        ack.Name = req.Name;
        ack.Carrer = req.Carrer;
        ack.AccountID = req.AccountID;
        NetworkManager.Instance.Send(MessageID.MSG_ROLE_CREATE_ACK, ack, 0, 0);
    }

    private void OnReq_RoleLogin(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        RoleLoginReq req = Serializer.Deserialize<RoleLoginReq>(ms);
        foreach(var current in DataDBSCharacter.Dict)
        {
            XCharacter c = current.Value;
            if(c.GUID == req.RoleID)
            {
                RoleLoginAck ack = new RoleLoginAck();
                ack.RoleID = c.GUID;
                ack.Name = c.Name;
                ack.Carrer = c.Carrer;
                ack.Level = c.Level;
                ack.Exp = c.CurExp;
                GTDataManager.Instance.LoadRoleData(c.Carrer);
                NetworkManager.Instance.Send(MessageID.MSG_ROLE_LOGIN_ACK, ack, 0, 0);


                NotifyIntoScene ntf = new NotifyIntoScene();
                ntf.CopyID = GTGlobal.LastCityID;
                ntf.CopyGuid = 0;
                ntf.RoleID = c.GUID;
                ntf.ServerID = MLLogin.Instance.GetCurrServerID();
                NetworkManager.Instance.Send(MessageID.MSG_NOTIFY_INTO_SCENE, ntf, 0, 0);
                break;
            }
        }
    }

    private void OnReq_EnterScene(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        EnterSceneReq req = Serializer.Deserialize<EnterSceneReq>(ms);
        EnterSceneAck ack = new EnterSceneAck();
        ack.CopyID = req.CopyID;
        ack.CopyGuid = req.CopyGuid;
        ack.Carrer = GTGlobal.CurPlayerCarrerID;
        ack.ActorID = ReadCfgActor.GetDataById(GTGlobal.CurPlayerCarrerID).Id;
        NetworkManager.Instance.Send(MessageID.MSG_ENTER_SCENE_ACK, ack, 0, 0);
    }

    private void OnReq_ObjectAction(MessageRecv obj)
    {
        SServer.Instance.SvrWorld.GetAOI(GTWorld.Main.Pos);
    }

    private void OnReq_MainCopy(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        MainCopyReq req = Serializer.Deserialize<MainCopyReq>(ms);
        FTEctypeData.CopyID = req.CopyID;
        MainCopyAck ack = new MainCopyAck();
        NetworkManager.Instance.Send(MessageID.MSG_MAIN_COPY_ACK, ack, 0, 0);
    }


    //**********************************             **********************************//
    private void OnReq_SortBag(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqSortBag req = Serializer.Deserialize<ReqSortBag>(ms);

        AckSortBag ack = new AckSortBag();
        ack.BagType = req.BagType;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_SORTBAG, ack, 0, 0);
    }

    private void OnReq_UnloadGem(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqUnloadGem req = Serializer.Deserialize<ReqUnloadGem>(ms);

        AckUnloadGem ack = new AckUnloadGem();
        ack.TarPos = req.TarPos;
        ack.Index  = req.Index;
        ack.NewPos = GTDataManager.Instance.GetNewPos(EBagType.GEM);
        NetworkManager.Instance.Send(MessageID.MSG_ACK_UNLOAD_GEM, ack, 0, 0);
    }

    private void OnReq_DressGem(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqDressGem req = Serializer.Deserialize<ReqDressGem>(ms);

        XItem bagGem = DataDBSBagGem.GetDataById(req.SrcPos);
        DGem cfg = ReadCfgGem.GetDataById(bagGem.Id);
        AckDressGem ack = new AckDressGem();
        ack.SrcPos = req.SrcPos;
        ack.Index  = req.Index;
        ack.TarPos = cfg.Pos;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_DRESS_GEM, ack, 0, 0);
    }

    private void OnReq_UnloadEquip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqUnloadEquip req = Serializer.Deserialize<ReqUnloadEquip>(ms);

        AckUnloadEquip ack = new AckUnloadEquip();
        ack.TarPos = req.TarPos;
        ack.NewPos = GTDataManager.Instance.GetNewPos(EBagType.ITEM);
        NetworkManager.Instance.Send(MessageID.MSG_ACK_UNLOAD_EQUIP, ack, 0, 0);
    }

    private void OnReq_DressEquip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqDressEquip req = Serializer.Deserialize<ReqDressEquip>(ms);

        AckDressEquip ack = new AckDressEquip();
        XItem bagEquip = DataDBSBagItem.GetDataById(req.SrcPos);
        DEquip equipDB = ReadCfgEquip.GetDataById(bagEquip.Id);
        ack.SrcPos = req.SrcPos;
        ack.TarPos = equipDB.Pos;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_DRESS_EQUIP, ack, 0, 0);
    }

    private void OnReq_UseItem(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqUseItem req = Serializer.Deserialize<ReqUseItem>(ms);

        AckUseItem ack = new AckUseItem();
        ack.Num = req.Num;
        ack.Pos = req.Pos;
        ack.PosType = req.PosType;
        ack.ID = req.ID;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_USEITEM, ack, 0, 0);
    }

    private void OnReq_ComposeChip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqComposeChip req = Serializer.Deserialize<ReqComposeChip>(ms);

        AckComposeChip ack = new AckComposeChip();
        ack.SrcPos = req.SrcPos;
        ack.Num = req.Num;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_COMPOSE_CHIP, ack, 0, 0);
    }

    private void OnReq_BuyStore(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqBuyStore req = Serializer.Deserialize<ReqBuyStore>(ms);

        AckBuyStore ack = new AckBuyStore();
        ack.StoreID     = req.StoreID;
        ack.Num         = req.Num;
        ack.StoreType   = req.StoreType;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_BUY_STORE, ack, 0, 0);
    }

    private void OnReq_StrengthGem(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqStrengthGem req = Serializer.Deserialize<ReqStrengthGem>(ms);

        AckStrengthGem ack = new AckStrengthGem();
        ack.TarGem = req.TarGem;
        ack.UseItems.AddRange(req.UseItems);
        NetworkManager.Instance.Send(MessageID.MSG_ACK_STRENGTHEN_GEM, ack, 0, 0);
    }

    private void OnReq_UpStarEquip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqUpStarEquip req = Serializer.Deserialize<ReqUpStarEquip>(ms);

        AckUpStarEquip ack = new AckUpStarEquip();
        ack.TarEquip = req.TarEquip;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_UPSTAR_EQUIP, ack, 0, 0);
    }

    private void OnReq_AdvanceEquip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqAdvanceEquip req = Serializer.Deserialize<ReqAdvanceEquip>(ms);

        DEquip        cfg = ReadCfgEquip.GetDataById(req.TarEquip.Id);
        int advanceID = cfg.Quality * 1000 + req.TarEquip.AdvanceLevel + 1;
        DEquipAdvanceCost db = ReadCfgEquipAdvanceCost.GetDataById(advanceID);

        AckAdvanceEquip ack = new AckAdvanceEquip();
        ack.UseItems.AddRange(req.UseItems);
        ack.TarEquip = req.TarEquip;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_ADVANCE_EQUIP, ack, 0, 0);
    }

    private void OnReq_StrengthEquip(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqStrengthEquip req = Serializer.Deserialize<ReqStrengthEquip>(ms);

        AckStrengthEquip ack = new AckStrengthEquip();
        ack.TarEquip = req.TarEquip;
        ack.UseItems.AddRange(req.UseItems);
        NetworkManager.Instance.Send(MessageID.MSG_ACK_STRENGTHEN_EQUIP, ack, 0, 0);
    }

    private void OnReq_SubmitTask(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqSubmitTask req = Serializer.Deserialize<ReqSubmitTask>(ms);

        AckSubmitTask ack = new AckSubmitTask();
        ack.ID   = req.ID;
        ack.Step = req.Step;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_SUBMIT_TASK, ack, 0, 0);
    }

    private void OnReq_UpgradePartner(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqUpgradePartner req = Serializer.Deserialize<ReqUpgradePartner>(ms);

        AckUpgradePartner ack = new AckUpgradePartner();
        ack.ID = req.ID;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_UPGRADE_PARTNER, ack, 0, 0);
    }

    private void OnReq_AdvancePartner(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqAdvancePartner req = Serializer.Deserialize<ReqAdvancePartner>(ms);

        AckAdvancePartner ack = new AckAdvancePartner();
        ack.ID = req.ID;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_ADVANVE_PARTNER, ack, 0, 0);
    }

    private void OnReq_ChangePartner(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqChangePartner req = Serializer.Deserialize<ReqChangePartner>(ms);

        AckChangePartner ack = new AckChangePartner();
        ack.ID = req.ID;
        ack.Pos = req.Pos;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_CHANGE_PARTNER, ack, 0, 0);
    }

    private void OnReq_UnloadPet(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqUnloadPet req = Serializer.Deserialize<ReqUnloadPet>(ms);

        AckUnloadPet ack = new AckUnloadPet();
        ack.ID = req.ID;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_UNLOAD_PET, ack, 0, 0);
    }

    private void OnReq_BattlePet(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqBattlePet req = Serializer.Deserialize<ReqBattlePet>(ms);

        AckBattlePet ack = new AckBattlePet();
        ack.ID = req.ID;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_BATTLE_PET, ack, 0, 0);
    }

    private void OnReq_UpgradePet(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqUpgradePet req = Serializer.Deserialize<ReqUpgradePet>(ms);

        AckUpgradePet ack = new AckUpgradePet();
        ack.ID = req.ID;
        ack.UseItems.AddRange(req.UseItems);
        NetworkManager.Instance.Send(MessageID.MSG_ACK_UPGRADE_PET, ack, 0, 0);
    }

    private void OnReq_UnloadRelics(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqUnloadRelics req = Serializer.Deserialize<ReqUnloadRelics>(ms);

        AckUnloadRelics ack = new AckUnloadRelics();
        ack.RelicsID = req.RelicsID;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_UNLOAD_RELICS, ack, 0, 0);
    }

    private void OnReq_BattleRelics(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqBattleRelics req = Serializer.Deserialize<ReqBattleRelics>(ms);

        AckBattleRelics ack = new AckBattleRelics();
        ack.RelicsID = req.RelicsID;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_BATTLE_RELICS, ack, 0, 0);
    }

    private void OnReq_UpgradeRelics(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqUpgradeRelics req = Serializer.Deserialize<ReqUpgradeRelics>(ms);

        AckUpgradeRelics ack = new AckUpgradeRelics();
        ack.RelicsID = req.RelicsID;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_UPGRADE_RELICS, ack, 0, 0);
    }

    private void OnReq_ChargeRelics(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqChargeRelics req = Serializer.Deserialize<ReqChargeRelics>(ms);

        AckChargeRelics ack = new AckChargeRelics();
        ack.RelicsID = req.RelicsID;
        ack.Index    = req.Index;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_CHARGE_RELICS, ack, 0, 0);
    }

    private void OnReq_SetMount(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ReqSetMount req = Serializer.Deserialize<ReqSetMount>(ms);

        AckSetMount ack = new AckSetMount();
        ack.MountID = req.MountID;
        NetworkManager.Instance.Send(MessageID.MSG_ACK_SETMOUNT, ack, 0, 0);
    }
}
