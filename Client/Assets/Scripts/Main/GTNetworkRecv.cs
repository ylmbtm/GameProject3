using UnityEngine;
using System.Collections;
using Protocol;
using ProtoBuf;
using System.Collections.Generic;
using MAP;

public class GTNetworkRecv : GTSingleton<GTNetworkRecv>
{
    public void AddListener()
    {
        NetworkManager.AddListener(MessageID.MSG_ACCOUNT_REG_ACK, OnAck_AccountRegister, this);
        NetworkManager.AddListener(MessageID.MSG_ACCOUNT_LOGIN_ACK, OnAck_AccountLogin, this);
        NetworkManager.AddListener(MessageID.MSG_SELECT_SERVER_ACK, OnAck_SelectServer, this);
        NetworkManager.AddListener(MessageID.MSG_ROLE_LIST_ACK, OnAck_GetRoleList, this);
        NetworkManager.AddListener(MessageID.MSG_SERVER_LIST_ACK, OnAck_GetServerList, this);
        NetworkManager.AddListener(MessageID.MSG_ROLE_CREATE_ACK, OnAck_CreateRole, this);
        NetworkManager.AddListener(MessageID.MSG_ROLE_LOGIN_ACK, OnAck_RoleLoginAck, this);
        NetworkManager.AddListener(MessageID.MSG_NOTIFY_INTO_SCENE, OnAck_NotifyIntoScene, this);
        NetworkManager.AddListener(MessageID.MSG_ENTER_SCENE_ACK, OnAck_EnterScene, this);

        NetworkManager.AddListener(MessageID.MSG_ACK_SORTBAG, OnAck_SortBag, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_DRESS_EQUIP, OnAck_DressEquip, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_UNLOAD_EQUIP, OnAck_UnloadEquip, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_DRESS_GEM, OnAck_DressGem, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_UNLOAD_GEM, OnAck_UnloadGem, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_COMPOSE_CHIP, OnAck_ComposeChip, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_USEITEM, OnAck_UseItem, this);

        NetworkManager.AddListener(MessageID.MSG_ACK_STRENGTHEN_EQUIP, OnAck_StrengthEquip, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_ADVANCE_EQUIP, OnAck_AdvanceEquip, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_UPSTAR_EQUIP, OnAck_UpStarEquip, this);

        NetworkManager.AddListener(MessageID.MSG_ACK_STRENGTHEN_GEM, OnAck_StrengthGem, this);

        NetworkManager.AddListener(MessageID.MSG_OBJECT_NEW_NTY, OnNty_ObjectNew, this);
        NetworkManager.AddListener(MessageID.MSG_OBJECT_REMOVE_NTY, OnNty_ObjectRemove, this);
        NetworkManager.AddListener(MessageID.MSG_OBJECT_ACTION_NTY, OnNty_ObjectAction, this);
        NetworkManager.AddListener(MessageID.MSG_ENTER_VIEW_NTF, OnNty_EnterView, this);
        NetworkManager.AddListener(MessageID.MSG_LEAVE_VIEW_NTF, OnNty_LeaveView, this);

        NetworkManager.AddListener(MessageID.MSG_ACK_SETMOUNT, OnAck_SetMount, this);

        NetworkManager.AddListener(MessageID.MSG_ACK_CHANGE_PARTNER, OnAck_ChangePartner, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_UPGRADE_PARTNER, OnAck_UpgradePartner, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_ADVANVE_PARTNER, OnAck_AdvancePartner, this);

        NetworkManager.AddListener(MessageID.MSG_ACK_BATTLE_PET, OnAck_BattlePet, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_UNLOAD_PET, OnAck_UnloadPet, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_UPGRADE_PET, OnAck_UpgradePet, this);

        NetworkManager.AddListener(MessageID.MSG_MAIN_COPY_ACK, On_MainCopyAck, this);

        NetworkManager.AddListener(MessageID.MSG_ACK_BATTLE_RELICS, OnAck_BattleRelics, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_UNLOAD_RELICS, OnAck_UnloadRelics, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_CHARGE_RELICS, OnAck_ChargeRelics, this);
        NetworkManager.AddListener(MessageID.MSG_ACK_UPGRADE_RELICS, OnAck_UpgradeRelics, this);

        NetworkManager.AddListener(MessageID.MSG_ACK_BUY_STORE, OnAck_BuyStore, this);
    }

    public void DelListener()
    {
        NetworkManager.DelListenerByTarget(this);
    }

    private void OnAck_AccountRegister(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AccountRegAck ack = Serializer.Deserialize<AccountRegAck>(ms);
        if (GTItemHelper.ShowNetworkError(ack.RetCode) == false)
        {
            return;
        }
        GTEventCenter.FireEvent(GTEventID.TYPE_LOGIN_ACCOUNT_REGISTER);
    }

    private void OnAck_AccountLogin(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AccountLoginAck ack = Serializer.Deserialize<AccountLoginAck>(ms);
        if (GTItemHelper.ShowNetworkError(ack.RetCode) == false)
        {
            return;
        }
        ClientServerNode newNode = new ClientServerNode();
        newNode.SvrID = ack.LastSvrID;
        newNode.SvrName = ack.LastSvrName;
        MLLogin.Instance.SetCurrServer(newNode);
        MLLogin.Instance.LastAccountID = ack.AccountID;
        GTEventCenter.FireEvent(GTEventID.TYPE_LOGIN_ACCOUNT_LOGIN);
    }

    private void OnAck_GetServerList(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ClientServerListAck ack = Serializer.Deserialize<ClientServerListAck>(ms);
        if (GTItemHelper.ShowNetworkError(ack.RetCode) == false)
        {
            return;
        }
        MLLogin.Instance.SetServerList(ack.SvrNode);
        GTEventCenter.FireEvent(GTEventID.TYPE_LOGIN_GETSERVERLIST);
    }

    private void OnAck_SelectServer(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        SelectServerAck ack = Serializer.Deserialize<SelectServerAck>(ms);
        if (GTItemHelper.ShowNetworkError(ack.RetCode) == false)
        {
            return;
        }
        GTEventCenter.FireEvent(GTEventID.TYPE_LOGIN_SELECTSERVER);
        if (GTGlobal.IsAlong)
        {
            GTLauncher.Instance.LoadScene(GTCopyKey.SCENE_Role);
        }
        else
        {
            NetworkManager.Instance.Close();
            NetworkManager.Instance.ConnectGameServer(ack.ServerAddr, ack.ServerPort, () =>
            {
                RoleListReq req = new RoleListReq();
                req.AccountID = MLLogin.Instance.LastAccountID;
                req.LoginCode = ack.LoginCode;
                NetworkManager.Instance.Send(MessageID.MSG_ROLE_LIST_REQ, req);
            });
        }
    }

    private void OnAck_GetRoleList(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        RoleListAck ack = Serializer.Deserialize<RoleListAck>(ms);
        if (GTItemHelper.ShowNetworkError(ack.RetCode) == false)
        {
            return;
        }
        for (int i = 0; i < ack.RoleList.Count; i++)
        {
            RoleItem item = ack.RoleList[i];
            XCharacter data = new XCharacter();
            data.Carrer = item.Carrer;
            data.Id = ReadCfgRole.GetDataById(item.Carrer).ActorID;
            data.GUID = item.RoleID;
            data.Level = item.Level;
            data.Name = item.Name;
            data.Type = (int)EActorType.PLAYER;
            data.Camp = (int)EBattleCamp.CT_PVE_PLAYER;
            DataDBSCharacter.Update(data.Carrer, data);
        }
        GTLauncher.Instance.LoadScene(GTCopyKey.SCENE_Role);
    }

    private void OnAck_CreateRole(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        RoleCreateAck ack = Serializer.Deserialize<RoleCreateAck>(ms);
        if (GTItemHelper.ShowNetworkError(ack.RetCode) == false)
        {
            return;
        }
        XCharacter data = new XCharacter();
        data.Carrer = ack.Carrer;
        data.Id = ReadCfgRole.GetDataById(ack.Carrer).ActorID;
        data.GUID = ack.RoleID;
        data.Name = ack.Name;
        data.Level = 1;
        data.Type = (int)EActorType.PLAYER;
        data.Camp = (int)EBattleCamp.CT_PVE_PLAYER;
        DataDBSCharacter.Insert(data.Carrer, data);
        GTEventCenter.FireEvent(GTEventID.TYPE_LOGIN_ROLECRATE);
    }

    private void OnAck_RoleLoginAck(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        RoleLoginAck ack = Serializer.Deserialize<RoleLoginAck>(ms);
        if (GTItemHelper.ShowNetworkError(ack.RetCode) == false)
        {
            return;
        }
        GTGlobal.CurPlayerCarrerID = ack.Carrer;
        GTGlobal.CurPlayerGUID = ack.RoleID;
        for (int i = 0; i < ack.Action.Count; i++)
        {
            int num = (int)ack.Action[i];
            int id = i + 101;
            DataDBSAction.Update(id, new XAction() { Id = id, Num = num });
        }
        GTDataTimer.Instance.Init();
        GTWorld.Instance.EnterGuide();
        GTWorld.Instance.Bie.UseGuide = GTLauncher.Instance.UseGuide;
        GTEventCenter.FireEvent(GTEventID.TYPE_LOGIN_ROLELOGIN);
    }

    private void OnAck_EnterScene(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        EnterSceneAck ack = Serializer.Deserialize<EnterSceneAck>(ms);
        if (GTItemHelper.ShowNetworkError(ack.RetCode) == false)
        {
            return;
        }
        XCharacter data = DataDBSCharacter.GetDataById(GTGlobal.CurPlayerCarrerID);
        if (data == null)
        {
            GTItemHelper.ShowTip("职业错误");
            return;
        }
        else
        {
            data.Id = ack.ActorID;
            data.PosX = ack.x;
            data.PosY = ack.y;
            data.PosZ = ack.z;
            data.Face = ack.vx;
            data.Type = (int)EActorType.PLAYER;
            data.Camp = ack.Camp;
            data.CurAttrs.AddRange(GTTools.GetListFromEnumNames<int>(typeof(EAttr)));
            data.CurAttrs[(int)EAttr.MAXHP - 1] = ack.hpmax;
            data.CurAttrs[(int)EAttr.MAXMP - 1] = ack.mpmax;
            data.CurAttrs[(int)EAttr.HP - 1] = ack.hp;
            data.CurAttrs[(int)EAttr.MP - 1] = ack.mp;
            data.CurAttrs[(int)EAttr.SPEED - 1] = 600;
            GTGlobal.Main = data;
            GTGlobal.CurCopyGUID = ack.CopyGuid;
            GTSimulateServer.CalcMainPropertys();
            GTWorld.Instance.ResetGuide();
            GTWorld.Instance.EnterWorld(ack.CopyID);

            GTEventCenter.FireEvent(GTEventID.TYPE_MAP_ENTERWORLD);
        }
    }

    private void OnAck_NotifyIntoScene(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        NotifyIntoScene ack = Serializer.Deserialize<NotifyIntoScene>(ms);
        GTLauncher.Instance.LoadScene(ack.CopyID, () =>
        {
            GTNetworkSend.Instance.TryEnterScene(ack.RoleID, ack.CopyID, ack.CopyGuid, ack.ServerID);
        });
    }

    private void OnAck_UseItem(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUseItem ack = Serializer.Deserialize<AckUseItem>(ms);

        int pos = ack.Pos;
        int num = ack.Num;
        int id = ack.ID;

        XItem item = DataDBSBagItem.GetDataById(pos);
        DItem itemDB = ReadCfgItem.GetDataById(item.Id);
        switch (itemDB.ItemType)
        {
            case EItemType.DRUG:
                GTDataManager.Instance.AddNewItem(itemDB.Data2, itemDB.Data1 * num);
                GTDataManager.Instance.UseItemById(itemDB.Id, num);
                break;
            case EItemType.BOX:
                break;
        }

        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_USE_ITEM, id, num);
    }

    private void OnAck_ComposeChip(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckComposeChip ack = Serializer.Deserialize<AckComposeChip>(ms);

        int srcPos = ack.SrcPos;
        int num = ack.Num;

        XItem item = DataDBSBagChip.Dict[srcPos];
        DItem itemDB = ReadCfgItem.GetDataById(item.Id);
        GTDataManager.Instance.AddNewItem(itemDB.Data2, num);
        GTDataManager.Instance.UseItemById(item.Id, itemDB.Data1 * num);

        GTItem data = new GTItem(itemDB.Data2, num);
        GTItemHelper.ShowAwardTip(data);

        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_COMPOSE_CHIP, srcPos);
    }

    private void OnAck_UnloadGem(MessageRecv obj)
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

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_CURRATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_UNLOAD_GEM, newPos, tarPos);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_DressGem(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckDressGem ack = Serializer.Deserialize<AckDressGem>(ms);

        int srcPos = ack.SrcPos;
        int tarPos = ack.TarPos;
        int index = ack.Index;

        XItem sourceGem = DataDBSBagGem.GetDataById(srcPos);
        DGem gemDB = ReadCfgGem.GetDataById(sourceGem.Id);
        int pos = MLGem.Instance.GetDressGemPosByIndexAndTargetPos(index, gemDB.Pos);
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

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_DRESS_GEM, srcPos, tarPos);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_UnloadEquip(MessageRecv obj)
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

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_EQUIP);

        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_UNLOAD_EQUIP, newPos, tarPos);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_DressEquip(MessageRecv obj)
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

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_EQUIP);

        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_DRESS_EQUIP, srcPos, tarPos);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_SortBag(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckSortBag ack = Serializer.Deserialize<AckSortBag>(ms);

        EBagType bagType = (EBagType)ack.BagType;
        Dictionary<int, XItem> items = GTDataManager.Instance.GetBagDataByBagType(bagType);
        List<XItem> list = new List<XItem>();
        list.AddRange(items.Values);
        items.Clear();

        switch (bagType)
        {
            case EBagType.ITEM:
                {
                    list.Sort(MLBag.Instance.CompareBagItem);
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
                    list.Sort(MLBag.Instance.CompareBagGem);
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
                    list.Sort(MLBag.Instance.CompareBagChip);
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
                    list.Sort(MLBag.Instance.CompareBagFashion);
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
                    list.Sort(MLBag.Instance.CompareBagRune);
                    DataDBSBagRune.Clear(false);
                    for (int i = 0; i < list.Count; i++)
                    {
                        list[i].Pos = i + 1;
                        DataDBSBagRune.Insert(list[i].Pos, list[i]);
                    }
                }
                break;
        }

        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_SORT_BAG);
    }

    private void OnAck_UpStarEquip(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUpStarEquip ack = Serializer.Deserialize<AckUpStarEquip>(ms);


        XEquip equip = ack.TarEquip;
        DEquip cfg = ReadCfgEquip.GetDataById(equip.Id);
        int starID = cfg.Quality * 1000 + equip.StarLevel + 1;

        DEquipStar db = ReadCfgEquipStar.GetDataById(starID);
        GTDataManager.Instance.UseMoney(db.CostMoneyId, db.CostMoneyNum);
        GTDataManager.Instance.UseItemById(db.CostItemId, db.CostItemNum);
        equip.StarLevel++;
        DataDBSEquip.Update(equip.Instance, equip);

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_UPSTAR_EQUIP);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_AdvanceEquip(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckAdvanceEquip ack = Serializer.Deserialize<AckAdvanceEquip>(ms);

        XEquip equip = ack.TarEquip;
        List<XItem> list = ack.UseItems;
        DEquip equipDB = ReadCfgEquip.GetDataById(equip.Id);
        int advanceID = equipDB.Quality * 1000 + equip.AdvanceLevel + 1;

        DEquipAdvanceCost db = ReadCfgEquipAdvanceCost.GetDataById(advanceID);
        GTDataManager.Instance.UseMoney(db.CostMoneyId, db.CostMoneyNum);
        GTDataManager.Instance.UseItemById(db.CostItemId, db.CostItemNum);
        if (list != null)
        {
            for (int i = 0; i < list.Count; i++)
            {
                GTDataManager.Instance.DelBagEquip(list[i].Pos);
            }
        }
        equip.AdvanceLevel++;
        DataDBSEquip.Update(equip.Instance, equip);

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_ADVANCE_EQUIP);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_StrengthEquip(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckStrengthEquip ack = Serializer.Deserialize<AckStrengthEquip>(ms);

        XEquip equip = ack.TarEquip;
        List<XItem> list = ack.UseItems;
        DEquip cfg = ReadCfgEquip.GetDataById(equip.Id);
        int strengthenLevel = equip.StrengthenLevel;
        int strID = cfg.Quality * 1000 + strengthenLevel + 1;
        int addExpNum = MLEquip.Instance.GetStrengthenExpByMatList(list);

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
        GTDataManager.Instance.UseMoney(DEquip.EQUIP_STRENGTHEN_MONEY_ID_1, expNum);
        for (int i = 0; i < list.Count; i++)
        {
            XItem item = list[i];
            if ((EPosType)item.PosType == EPosType.RoleMoney)
            {
                GTDataManager.Instance.UseMoney(item.Id, item.Num);
            }
            else
            {
                GTDataManager.Instance.DelBagEquip(item.Pos);
            }
        }

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_STRENGTHEN_EQUIP);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_StrengthGem(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckStrengthGem ack = Serializer.Deserialize<AckStrengthGem>(ms);
        XGem gem = ack.TarGem;
        List<XItem> list = ack.UseItems;

        DGem gemDB = ReadCfgGem.GetDataById(gem.Id);
        int addExpNum = MLGem.Instance.GetExpByStrengthMatList(list);
        int strengthenLevel = gem.StrengthenLevel;

        int levelID = gemDB.Quality * 1000 + strengthenLevel + 1;
        DGemLevel db = ReadCfgGemLevel.GetDataById(levelID);
        int expNum = db.RequireExp - gem.StrengthenExp;
        while (addExpNum >= expNum)
        {
            strengthenLevel++;
            levelID = gemDB.Quality * 1000 + strengthenLevel + 1;
            db = ReadCfgGemLevel.GetDataById(levelID);
            expNum += db.RequireExp;
        }
        gem.StrengthenLevel = strengthenLevel;
        gem.StrengthenExp = addExpNum + db.RequireExp - expNum;

        DataDBSGem.Update(gem.Instance, gem);
        GTDataManager.Instance.UseMoney(db.CostMoneyId, expNum);
        for (int i = 0; i < list.Count; i++)
        {
            XItem item = list[i];
            EPosType posType = (EPosType)item.PosType;
            switch (posType)
            {
                case EPosType.BagItem:
                    GTDataManager.Instance.UseItemById(item.Id, item.Num);
                    break;
                case EPosType.BagGem:
                    GTDataManager.Instance.DelBagGem(item.Pos);
                    break;
            }
        }

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_BAG_STRENGTHEN_GEM);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnNty_ObjectNew(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ObjectNewNty nty = Serializer.Deserialize<ObjectNewNty>(ms);
        for (int i = 0; i < nty.NewList.Count; i++)
        {
            NewItem item = nty.NewList[i];
            XCharacter data = new XCharacter();
            data.Id = item.ActorID;
            data.GUID = item.ObjectGuid;
            data.Level = item.Level;
            data.Name = item.Name;
            switch ((OBJECT_TYPE)item.ObjType)
            {
                case OBJECT_TYPE.OT_PLAYER:
                    data.Type = (int)EActorType.PLAYER;
                    break;
                case OBJECT_TYPE.OT_MONSTER:
                    data.Type = (int)EActorType.MONSTER;
                    break;
            }
            data.PosX = item.x;
            data.PosY = item.y;
            data.PosZ = item.z;
            data.Face = Quaternion.FromToRotation(Vector3.forward, new Vector3(item.x, item.y, item.z)).eulerAngles.y;
            data.CurAttrs.AddRange(GTTools.GetListFromEnumNames<int>(typeof(EAttr)));
            data.CurAttrs[(int)EAttr.MAXHP - 1] = item.hpmax;
            data.CurAttrs[(int)EAttr.MAXMP - 1] = item.mpmax;
            data.CurAttrs[(int)EAttr.HP - 1] = item.hp;
            data.CurAttrs[(int)EAttr.MP - 1] = item.mp;
            data.CurAttrs[(int)EAttr.SPEED - 1] = 400;
            Character cc = GTWorld.Instance.AddActorSync(data);
        }
    }

    private void OnNty_ObjectRemove(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ObjectRemoveNty Nty = Serializer.Deserialize<ObjectRemoveNty>(ms);
        for (int i = 0; i < Nty.RemoveList.Count; i++)
        {
            GTWorld.Instance.DelActor(Nty.RemoveList[i]);
        }
    }

    private void OnNty_ObjectAction(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ObjectActionNty nty = Serializer.Deserialize<ObjectActionNty>(ms);
        for (int i = 0; i < nty.ActionList.Count; i++)
        {
            ActionItem item = nty.ActionList[i];
            Character cc = GTWorld.Instance.GetActor(item.ObjectGuid);
            if (cc == null)
            {
                continue;
            }
            cc.CurrAttr.HP = (int)item.hp;
            cc.CurrAttr.MP = (int)item.mp;
            cc.CurrAttr.MaxHP = (int)item.hpmax;
            cc.CurrAttr.MaxMP = (int)item.mpmax;
            if (cc.GUID == GTGlobal.CurPlayerGUID)
            {
                continue;
            }
            Vector3 toTarget = new Vector3(item.x, 0, item.z);
            Vector3 toTowards = new Vector3(item.vx, 0, item.vz);
            switch ((ACTION_TYPE)item.ActionID)
            {
                case ACTION_TYPE.AT_IDLE:
                    cc.Action.Play("idle", null);
                    cc.Sync.SyncIdle(toTarget, toTowards);
                    break;
                case ACTION_TYPE.AT_MOVE:
                    cc.Action.Play("idle", null);
                    cc.Sync.SyncIdle(toTarget, toTowards);
                    break;
                case ACTION_TYPE.AT_RUN:
                    cc.Action.Play("run", null);
                    cc.Sync.SyncPosition(toTarget, toTowards);
                    break;
                case ACTION_TYPE.AT_WALK:
                    cc.Action.Play("walk", null);
                    cc.Sync.SyncPosition(toTarget, toTowards);
                    break;
            }
        }
    }

    private void OnNty_EnterView(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        EnterViewNtf nty = Serializer.Deserialize<EnterViewNtf>(ms);
        for (int i = 0; i < nty.List.Count; i++)
        {
            Character cc = GTWorld.Instance.AddActorLocalSync(nty.List[i]);
        }
    }

    private void OnNty_LeaveView(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        LeaveViewNtf Nty = Serializer.Deserialize<LeaveViewNtf>(ms);
        for (int i = 0; i < Nty.List.Count; i++)
        {
            GTWorld.Instance.DelActor(Nty.List[i]);
        }
    }


    private void OnAck_SetMount(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckSetMount ack = Serializer.Deserialize<AckSetMount>(ms);
        XCharacter role = GTGlobal.Main;
        role.Mount = ack.MountID;
        DataDBSCharacter.Update(role.Carrer, role);
        GTEventCenter.FireEvent(GTEventID.TYPE_MOUNT_DRESSMOUNT);
    }

    private void OnAck_AdvancePartner(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckAdvancePartner ack = Serializer.Deserialize<AckAdvancePartner>(ms);

        XPartner partner = DataDBSPartner.GetDataById(ack.ID);
        if (partner == null)
        {
            partner = new XPartner();
            partner.Id = ack.ID;
            partner.Advance = 1;
        }
        else
        {
            partner.Advance++;
        }
        DataDBSPartner.Update(ack.ID, partner);
        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_PARTNER_ADVANCE);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_UpgradePartner(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUpgradePartner ack = Serializer.Deserialize<AckUpgradePartner>(ms);

        XPartner partner = DataDBSPartner.GetDataById(ack.ID);
        if (partner == null)
        {
            partner = new XPartner();
            partner.Id = ack.ID;
            partner.Level = 1;
        }
        else
        {
            partner.Level++;
        }
        DataDBSPartner.Update(ack.ID, partner);
        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_PET_UPGRADE);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_ChangePartner(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckChangePartner ack = Serializer.Deserialize<AckChangePartner>(ms);

        XCharacter role = GTGlobal.Main;
        switch (ack.Pos)
        {
            case 1:
                role.Partner1 = ack.ID;
                break;
            case 2:
                role.Partner2 = ack.ID;
                break;
        }
        DataDBSCharacter.Update(role.Carrer, role);
        if (!DataDBSPartner.ContainsKey(ack.ID))
        {
            XPartner xp = new XPartner();
            xp.Id = ack.ID;
            xp.Level = 1;
            xp.Star = 0;
            xp.Wake = 0;
            xp.Advance = 1;
            xp.Exp = 0;
            DataDBSPartner.Insert(ack.ID, xp);
        }

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_PARTNER_CHANGE, ack.Pos, ack.ID);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_UpgradePet(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUpgradePet ack = Serializer.Deserialize<AckUpgradePet>(ms);
        int id = ack.ID;
        List<XItem> items = ack.UseItems;

        DActor db = ReadCfgActor.GetDataById(id);
        XPet data = DataDBSPet.GetDataById(id);
        if (data == null)
        {
            data = GTDataFactory.CreatePet(id);
        }

        int exp = MLPet.Instance.GetExpByItemList(items);
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
            GTDataManager.Instance.UseItemById(items[i].Id, 1);
        }

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);

        GTEventCenter.FireEvent(GTEventID.TYPE_PET_UPGRADE);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_UnloadPet(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUnloadPet ack = Serializer.Deserialize<AckUnloadPet>(ms);

        XCharacter role = GTGlobal.Main;
        role.PetID = 0;
        DataDBSCharacter.Update(role.Carrer, role);

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_PET_UNLOAD);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_BattlePet(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckBattlePet ack = Serializer.Deserialize<AckBattlePet>(ms);

        XCharacter role = GTGlobal.Main;
        role.PetID = ack.ID;
        DataDBSCharacter.Update(role.Carrer, role);

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_PET_BATTLE);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void On_MainCopyAck(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        MainCopyAck ack = Serializer.Deserialize<MainCopyAck>(ms);
        if (ack.RetCode != 0)
        {
            return;
        }
        GTEventCenter.FireEvent(GTEventID.TYPE_ECTYPE_BATTLECHECK);
        GTLauncher.Instance.LoadScene(FTEctypeData.CopyID);
    }

    private void OnAck_UpgradeRelics(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUpgradeRelics ack = Serializer.Deserialize<AckUpgradeRelics>(ms);


        DRelics db = ReadCfgRelics.GetDataById(ack.RelicsID);
        XRelics relics = DataDBSRelics.GetDataById(ack.RelicsID);
        relics.Level++;
        relics.CurExp1 = relics.CurExp1 - db.LevelExp[relics.Level - 1];
        relics.CurExp2 = relics.CurExp2 - db.LevelExp[relics.Level - 1];
        relics.CurExp3 = relics.CurExp3 - db.LevelExp[relics.Level - 1];
        DataDBSRelics.Update(ack.RelicsID, relics);

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_RELICS_UPGRADE, ack.RelicsID);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_ChargeRelics(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckChargeRelics ack = Serializer.Deserialize<AckChargeRelics>(ms);
        int index = ack.Index;
        int relicsID = ack.RelicsID;

        DRelics db = ReadCfgRelics.GetDataById(relicsID);
        int itemID = db.ArtificeCostID[index - 1];
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        XRelics relics = DataDBSRelics.GetDataById(relicsID);
        if (relics == null)
        {
            relics = new XRelics();
            relics.Id = relicsID;
        }
        switch (index)
        {
            case 1:
                relics.CurExp1 += itemDB.Data1;
                break;
            case 2:
                relics.CurExp2 += itemDB.Data1;
                break;
            case 3:
                relics.CurExp3 += itemDB.Data1;
                break;
        }

        DataDBSRelics.Update(relicsID, relics);
        GTDataManager.Instance.UseItemById(itemID);

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_RELICS_CHARGE, relicsID, index);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_UnloadRelics(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckUnloadRelics ack = Serializer.Deserialize<AckUnloadRelics>(ms);

        XCharacter role = GTGlobal.Main;
        role.Relic = 0;
        DataDBSCharacter.Update(role.Carrer, role);

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_RELICS_UNLOAD, ack.RelicsID);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_BattleRelics(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckBattleRelics ack = Serializer.Deserialize<AckBattleRelics>(ms);

        XCharacter role = GTGlobal.Main;
        role.Relic = ack.RelicsID;
        DataDBSCharacter.Update(role.Carrer, role);

        GTWorld.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_RELICS_BATTLE, ack.RelicsID);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }

    private void OnAck_BuyStore(MessageRecv obj)
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckBuyStore ack = Serializer.Deserialize<AckBuyStore>(ms);

        DStore db = ReadCfgStore.GetDataById(ack.StoreID);
        GTItem data = new GTItem(db.ItemID, db.ItemNum * ack.Num);
        GTItemHelper.ShowAwardTip(data);
        GTDataManager.Instance.AddNewItem(db.ItemID, db.ItemNum * ack.Num);
        GTDataManager.Instance.UseMoney(db.CostMoneyID, db.CostMoneyNum * ack.Num);

        GTEventCenter.FireEvent(GTEventID.TYPE_STORE_BUYSUCCESS);
    }
}
