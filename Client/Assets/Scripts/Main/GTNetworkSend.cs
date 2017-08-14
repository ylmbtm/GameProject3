using UnityEngine;
using System.Collections;
using System;
using ACT;
using Protocol;
using System.Collections.Generic;

public class GTNetworkSend : GTSingleton<GTNetworkSend>
{
    public void TryRegister(string username, string password)
    {
        AccountRegReq req = new AccountRegReq();
        req.AccountName = username;
        req.Password = password;
        MLLogin.Instance.LastUsername = username;
        MLLogin.Instance.LastPassword = password;
        NetworkManager.Instance.Send(MessageID.MSG_ACCOUNT_REG_REQ, req);
    }

    public void TryLogin(string username, string password)
    {
        AccountLoginReq req = new AccountLoginReq();
        req.AccountName = username;
        req.Password = password;
        NetworkManager.Instance.Send(MessageID.MSG_ACCOUNT_LOGIN_REQ, req, 0, 0);
    }

    public void TryGetSvrList()
    {
        ClientServerListReq req = new ClientServerListReq();
        req.AccountID = MLLogin.Instance.LastAccountID;
        req.Channel = 2;
        req.ClientVersion = 100001;
        NetworkManager.Instance.Send(MessageID.MSG_SERVER_LIST_REQ, req, 0, 0);
    }

    //确认当前服务器
    public void TrySelectServer(int ServerID)
    {
        SelectServerReq req = new SelectServerReq();
        req.ServerID = ServerID;
        NetworkManager.Instance.Send(MessageID.MSG_SELECT_SERVER_REQ, req);
    }

    //创建角色
    public void TryCreateRole(string name, int carrerID, ulong accountID)
    {
        if (string.IsNullOrEmpty(name))
        {
            GTItemHelper.ShowTip("名字不能为空");
            return;
        }
        RoleCreateReq req = new RoleCreateReq();
        req.Name = name;
        req.AccountID = accountID;
        req.Carrer = carrerID;
        NetworkManager.Instance.Send(MessageID.MSG_ROLE_CREATE_REQ, req);
    }

    //取角色列表
    public void TryGetRoleList()
    {
        RoleListReq req = new RoleListReq();
        req.AccountID = MLLogin.Instance.LastAccountID;
        req.LoginCode = 0x111;
        NetworkManager.Instance.Send(MessageID.MSG_ROLE_LIST_REQ, req);
    }

    //登录角色
    public void TryRoleLogin(UInt64 roleGUID)
    {
        RoleLoginReq req = new RoleLoginReq();
        req.AccountID = MLLogin.Instance.LastAccountID;
        req.RoleID = roleGUID;
        req.LoginCode = 0x111;
        NetworkManager.Instance.Send(MessageID.MSG_ROLE_LOGIN_REQ, req);
    }

    //进入副本场景或者主城场景
    public void TryEnterScene(UInt64 roleID, Int32 copyID, Int32 copyGUID, Int32 ServerID)
    {
        EnterSceneReq req = new EnterSceneReq();
        req.RoleID = roleID;
        req.CopyID = copyID;
        req.ServerID = ServerID;
        req.CopyGuid = copyGUID;
        NetworkManager.Instance.Send(MessageID.MSG_ENTER_SCENE_REQ, req, (UInt32)ServerID, (UInt32)copyGUID);
    }

    public void TrySyncMove()
    {
        ActionItem data = new ActionItem();
        data.ActionID = (int)ACTION_TYPE.AT_RUN;
        data.x = GTWorld.Main.Pos.x;
        data.y = GTWorld.Main.Pos.y;
        data.z = GTWorld.Main.Pos.z;
        data.vx = GTWorld.Main.Dir.x;
        data.vz = GTWorld.Main.Dir.z;
        data.ObjectGuid = GTWorld.Main.GUID;
        ObjectActionReq req = new ObjectActionReq();
        req.ActionList.Add(data);
        NetworkManager.Instance.Send(MessageID.MSG_OBJECT_ACTION_REQ, req, GTGlobal.CurPlayerGUID, (uint)GTGlobal.CurCopyGUID);
    }

    public void TrySyncIdle()
    {
        ActionItem data = new ActionItem();
        data.ActionID = (int)ACTION_TYPE.AT_IDLE;
        data.x = GTWorld.Main.CacheTransform.position.x;
        data.z = GTWorld.Main.CacheTransform.position.z;
        data.vx = GTWorld.Main.Dir.x;
        data.vz = GTWorld.Main.Dir.z;
        data.ObjectGuid = GTWorld.Main.GUID;
        ObjectActionReq req = new ObjectActionReq();
        req.ActionList.Add(data);
        NetworkManager.Instance.Send(MessageID.MSG_OBJECT_ACTION_REQ, req, GTGlobal.CurPlayerGUID, (uint)GTGlobal.CurCopyGUID);
    }

    public void TryChangeMount(int mountID)
    {
        ReqSetMount req = new ReqSetMount();
        req.MountID = mountID;
        NetworkManager.Instance.Send<ReqSetMount>(MessageID.MSG_REQ_SETMOUNT, req);
    }

    public void TryChangePartner(int pos, int id)
    {
        ReqChangePartner req = new ReqChangePartner();
        req.ID = id;
        req.Pos = pos;
        NetworkManager.Instance.Send<ReqChangePartner>(MessageID.MSG_REQ_CHANGE_PARTNER, req);
    }

    public void TryUpgradePartner(int id)
    {
        ReqUpgradePartner req = new ReqUpgradePartner();
        req.ID = id;
        NetworkManager.Instance.Send<ReqUpgradePartner>(MessageID.MSG_REQ_UPGRADE_PARTNER, req);
    }

    public void TryAdvancePartner(int id)
    {
        ReqAdvancePartner req = new ReqAdvancePartner();
        req.ID = id;
        NetworkManager.Instance.Send<ReqAdvancePartner>(MessageID.MSG_REQ_ADVANVE_PARTNER, req);
    }

    public void TryUpgradePet(int id, List<XItem> items)
    {
        if (items == null || items == null)
        {
            GTItemHelper.ShowTip("缺少食物");
            return;
        }
        for (int i = 0; i < items.Count; i++)
        {
            if (MLPet.Instance.IsFood(items[i].Id) == false)
            {
                GTItemHelper.ShowTip("加入的食品列表有误");
                return;
            }
        }
        DActor db = ReadCfgActor.GetDataById(id);
        XPet data = DataDBSPet.GetDataById(id);
        if (data != null)
        {
            int count = 0;
            foreach (var current in ReadCfgPetLevel.Dict)
            {
                if (current.Value.Quality == db.Quality)
                {
                    count++;
                }
            }

            if (data.Level >= count)
            {
                GTItemHelper.ShowTip("宠物等级已满");
                return;
            }
        }

        ReqUpgradePet req = new ReqUpgradePet();
        req.ID = id;
        req.UseItems.AddRange(items);
        NetworkManager.Instance.Send<ReqUpgradePet>(MessageID.MSG_REQ_UPGRADE_PET, req);
    }

    public void TryBattlePet(int id)
    {
        XCharacter role = GTGlobal.Main;
        if (role.PetID == id)
        {
            GTItemHelper.ShowTip("你的伙伴已上阵");
            return;
        }

        ReqBattlePet req = new ReqBattlePet();
        req.ID = id;
        NetworkManager.Instance.Send<ReqBattlePet>(MessageID.MSG_REQ_BATTLE_PET, req);
    }

    public void TryUnloadPet(int id)
    {
        XCharacter role = GTGlobal.Main;
        if (role.PetID != id)
        {
            GTItemHelper.ShowTip("你没有上阵该伙伴");
            return;
        }

        ReqUnloadPet req = new ReqUnloadPet();
        req.ID = id;
        NetworkManager.Instance.Send<ReqUnloadPet>(MessageID.MSG_REQ_UNLOAD_PET, req);
    }

    public void TrySortBag(EBagType bagType)
    {
        ReqSortBag req = new ReqSortBag();
        req.BagType = (int)bagType;
        NetworkManager.Instance.Send<ReqSortBag>(MessageID.MSG_REQ_SORTBAG, req);
    }

    public void TryUseItemByPos(int pos, int num)
    {
        XItem item = DataDBSBagItem.GetDataById(pos);
        if (item.Num < num)
        {
            GTItemHelper.ShowTip("物品不足");
            return;
        }
        DItem itemDB = ReadCfgItem.GetDataById(item.Id);
        if (itemDB.ItemType == EItemType.BOX)
        {
            if (itemDB.Data1 != 0)
            {
                if (GTDataManager.Instance.GetItemCountById(itemDB.Data1) < num)
                {
                    GTItemHelper.ShowTip("需要足够的钥匙");
                    return;
                }
            }
            DAward awardDB = ReadCfgAward.GetDataById(itemDB.Data2);
            if (GTItemHelper.CheckBagFull(awardDB.MaxDropNum))
            {
                return;
            }
        }

        ReqUseItem req = new ReqUseItem();
        req.Pos = pos;
        req.Num = num;
        req.PosType = (int)EPosType.BagItem;
        req.ID = item.Id;
        NetworkManager.Instance.Send<ReqUseItem>(MessageID.MSG_REQ_USEITEM, req);
    }

    public void TryDressEquip(int srcPos)
    {
        ReqDressEquip req = new ReqDressEquip();
        req.SrcPos = srcPos;
        NetworkManager.Instance.Send<ReqDressEquip>(MessageID.MSG_REQ_DRESS_EQUIP, req);
    }

    public void TryUnloadEquip(int tarPos)
    {
        if (GTItemHelper.CheckBagFull(1, EBagType.ITEM))
        {
            return;
        }
        ReqUnloadEquip req = new ReqUnloadEquip();
        req.TarPos = tarPos;
        NetworkManager.Instance.Send<ReqUnloadEquip>(MessageID.MSG_REQ_UNLOAD_EQUIP, req);
    }

    public void TryDressGem(int index, int srcPos)
    {
        ReqDressGem req = new ReqDressGem();
        req.SrcPos = srcPos;
        req.Index = index;
        NetworkManager.Instance.Send<ReqDressGem>(MessageID.MSG_REQ_DRESS_GEM, req);
    }

    public void TryUnloadGem(int index, int tarPos)
    {
        if (GTItemHelper.CheckBagFull(1, EBagType.GEM))
        {
            return;
        }

        ReqUnloadGem req = new ReqUnloadGem();
        req.TarPos = tarPos;
        req.Index = index;
        NetworkManager.Instance.Send<ReqUnloadGem>(MessageID.MSG_REQ_UNLOAD_GEM, req);
    }

    public void TryComposeChip(int srcPos)
    {
        XItem item = DataDBSBagChip.Dict[srcPos];
        DItem itemDB = ReadCfgItem.GetDataById(item.Id);
        int composeNum = item.Num / itemDB.Data1;
        if (GTItemHelper.CheckBagFull(composeNum, EBagType.GEM))
        {
            return;
        }
        ReqComposeChip req = new ReqComposeChip();
        req.SrcPos = srcPos;
        req.Num = composeNum;
        NetworkManager.Instance.Send<ReqComposeChip>(MessageID.MSG_REQ_COMPOSE_CHIP, req);
    }

    public void TryOneKeyToDressGem(int index)
    {
        ReqOneKeyToDressGem req = new ReqOneKeyToDressGem();
        req.Index = index;
        NetworkManager.Instance.Send<ReqOneKeyToDressGem>(MessageID.MSG_REQ_ONEKEYTODRESSGEM, req);
    }

    public void TryOneKeyToUnloadGem(int index)
    {
        ReqOneKeyToUnloadGem req = new ReqOneKeyToUnloadGem();
        req.Index = index;
        NetworkManager.Instance.Send<ReqOneKeyToUnloadGem>(MessageID.MSG_REQ_ONEKEYTOUNLOADGEM, req);
    }

    public void TryStrengthEquip(XEquip equip, List<XItem> list)
    {
        if (list.Count == 0)
        {
            GTItemHelper.ShowTip("请添加装备或者水晶");
            return;
        }
        int addExpNum = MLEquip.Instance.GetStrengthenExpByMatList(list);
        if (!GTItemHelper.CheckItemEnongh(DEquip.EQUIP_STRENGTHEN_MONEY_ID_1, addExpNum))
        {
            return;
        }

        ReqStrengthEquip req = new ReqStrengthEquip();
        req.TarEquip = equip;
        req.UseItems.AddRange(list);
        NetworkManager.Instance.Send<ReqStrengthEquip>(MessageID.MSG_REQ_STRENGTHEN_EQUIP, req);
    }

    public void TryAdvanceEquip(XEquip equip)
    {
        if (MLEquip.Instance.IsFullAdvanceLevel(equip))
        {
            GTItemHelper.ShowTip("进阶等级已满");
            return;
        }
        DEquip cfg = ReadCfgEquip.GetDataById(equip.Id);
        int advanceID = cfg.Quality * 1000 + equip.AdvanceLevel + 1;
        DEquipAdvanceCost db = ReadCfgEquipAdvanceCost.GetDataById(advanceID);

        if (!GTItemHelper.CheckItemEnongh(db.CostMoneyId, db.CostMoneyNum))
        {
            return;
        }
        if (!GTItemHelper.CheckItemEnongh(db.CostItemId, db.CostItemNum))
        {
            return;
        }

        List<XItem> list;
        if (db.CostEquipNum > 0)
        {
            list = MLEquip.Instance.GetBagSameEquipList(equip);
            if (list.Count < db.CostEquipNum)
            {
                GTItemHelper.ShowTip("缺少同样的装备");
                return;
            }
            list.RemoveRange(db.CostEquipNum, list.Count - db.CostEquipNum);
        }
        else
        {
            list = new List<XItem>();
        }

        ReqAdvanceEquip req = new ReqAdvanceEquip();
        req.TarEquip = equip;
        req.UseItems.AddRange(list);
        NetworkManager.Instance.Send<ReqAdvanceEquip>(MessageID.MSG_REQ_ADVANCE_EQUIP, req);
    }

    public void TryUpStarEquip(XEquip equip)
    {
        if (MLEquip.Instance.IsFullStarLevel(equip))
        {
            GTItemHelper.ShowTip("装备星级已满");
            return;
        }
        DEquip equipDB = ReadCfgEquip.GetDataById(equip.Id);
        int starID = equipDB.Quality * 1000 + equip.StarLevel + 1;
        DEquipStar db = ReadCfgEquipStar.GetDataById(starID);
        if (!GTItemHelper.CheckItemEnongh(db.CostMoneyId, db.CostMoneyNum))
        {
            return;
        }
        if (!GTItemHelper.CheckItemEnongh(db.CostItemId, db.CostItemNum))
        {
            return;
        }

        ReqUpStarEquip req = new ReqUpStarEquip();
        req.TarEquip = equip;
        NetworkManager.Instance.Send<ReqUpStarEquip>(MessageID.MSG_REQ_UPSTAR_EQUIP, req);
    }

    public void TryStrengthGem(XGem gem, List<XItem> list)
    {
        if (MLGem.Instance.IsFullStrengthenLevel(gem))
        {
            GTItemHelper.ShowTip("宝石强化等级已满");
            return;
        }
        if (list.Count == 0)
        {
            GTItemHelper.ShowTip("请添加升级材料");
            return;
        }
        int exp = MLGem.Instance.GetExpByStrengthMatList(list);
        if (GTItemHelper.CheckItemEnongh(1, exp) == false)
        {
            return;
        }

        ReqStrengthGem req = new ReqStrengthGem();
        req.TarGem = gem;
        req.UseItems.AddRange(list);
        NetworkManager.Instance.Send<ReqStrengthGem>(MessageID.MSG_REQ_STRENGTHEN_GEM, req);
    }

    public void TryChargeRelics(int relicsID, int index)
    {
        DRelics db = ReadCfgRelics.GetDataById(relicsID);
        if (db == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        if (index > 3 || index < 1)
        {
            GTItemHelper.ShowTip("非法索引" + index.ToString());
            return;
        }
        XRelics relics = DataDBSRelics.GetDataById(relicsID);
        if (relics != null)
        {
            if (MLRelics.Instance.GetExp(relics, index) >= db.LevelExp[relics.Level])
            {
                GTItemHelper.ShowTip("你不能对此充能了");
                return;
            }
        }
        if (GTItemHelper.CheckItemEnongh(db.ArtificeCostID[index - 1], 1) == false)
        {
            return;
        }

        ReqChargeRelics req = new ReqChargeRelics();
        req.RelicsID = relicsID;
        req.Index = index;
        NetworkManager.Instance.Send(MessageID.MSG_REQ_CHARGE_RELICS, req);
    }

    public void TryUpgradeRelics(int relicsID)
    {
        DRelics db = ReadCfgRelics.GetDataById(relicsID);
        if (db == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        XRelics relics = DataDBSRelics.GetDataById(relicsID);
        if (relics == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        if (relics.Level == db.LevelExp.Length)
        {
            GTItemHelper.ShowTip("已升到最大等级，无法继续升级");
            return;
        }
        for (int i = 0; i < 3; i++)
        {
            if (MLRelics.Instance.GetExp(relics, i + 1) < db.LevelExp[relics.Level])
            {
                GTItemHelper.ShowTip("神器未获得足够的充能");
                return;
            }
        }
        ReqUpgradeRelics req = new ReqUpgradeRelics();
        req.RelicsID = relicsID;
        NetworkManager.Instance.Send(MessageID.MSG_REQ_UPGRADE_RELICS, req);
    }

    public void TryBattleRelics(int relicsID)
    {
        DRelics db = ReadCfgRelics.GetDataById(relicsID);
        if (db == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        XCharacter role = GTGlobal.Main;
        if (role.Relic == relicsID)
        {
            GTItemHelper.ShowTip("该神器已经上阵");
            return;
        }

        ReqBattleRelics req = new ReqBattleRelics();
        req.RelicsID = relicsID;
        NetworkManager.Instance.Send(MessageID.MSG_REQ_BATTLE_RELICS, req);
    }

    public void TryUnloadRelics(int relicsID)
    {
        DRelics db = ReadCfgRelics.GetDataById(relicsID);
        if (db == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        XCharacter role = GTGlobal.Main;
        if (role.Relic != relicsID)
        {
            GTItemHelper.ShowTip("该神器没有上阵，无法卸下");
            return;
        }
        ReqUnloadRelics req = new ReqUnloadRelics();
        req.RelicsID = relicsID;
        NetworkManager.Instance.Send(MessageID.MSG_REQ_UNLOAD_RELICS, req);
    }

    public void TryBuyStore(int storeType, int storeID, int num)
    {
        DStore storeDB = ReadCfgStore.GetDataById(storeID);
        if (storeDB == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        if (GTItemHelper.CheckItemEnongh(storeDB.CostMoneyID, storeDB.CostMoneyNum) == false)
        {
            return;
        }
        if (GTItemHelper.CheckBagFull(1))
        {
            return;
        }
        ReqBuyStore req = new ReqBuyStore();
        req.StoreType = storeType;
        req.StoreID = storeID;
        req.Num = num;
        NetworkManager.Instance.Send<ReqBuyStore>(MessageID.MSG_REQ_BUY_STORE, req);
    }

    public void TryCastSkill(ESkillPos pos)
    {
        if (GTWorld.Main == null || GTWorld.Main.CacheTransform == null)
        {
            return;
        }
        ActSkill skill = GTWorld.Main.Skill.GetSkill(pos);
        if (skill == null)
        {
            return;
        }
        if (GTWorld.Main.Target != null && GTWorld.Main.Target.IsDead())
        {
            CharacterHelper.ClearTarget(GTWorld.Main);
        }
        if (GTWorld.Main.Target == null)
        {
            Character target = GTWorld.Main.FindEnemyNearest(GTWorld.Main.Skill.WarnerDist);
            CharacterHelper.SetTarget(GTWorld.Main, target);
        }
        GTWorld.Main.PausedAI(true);
        Resp resp = GTWorld.Main.Command.Get<CommandUseSkill>().Update(skill.ID).Do();
        CharacterHelper.CalcCharacterOperateError(resp);
        if (resp == Resp.TYPE_YES)
        {
            GTWorld.Main.UsePowerByCostType(skill.CostType, skill.CostNum);
            GTEventCenter.FireEvent(GTEventID.TYPE_UPDATE_AVATAR_HP);
            GTEventCenter.FireEvent(GTEventID.TYPE_UPDATE_AVATAR_MP);
        }
        GTWorld.Main.PausedAI(false);
    }

    public void TryStartMount()
    {
        XCharacter data = GTGlobal.Main;
        if (data.Mount == 0)
        {
            CharacterHelper.CalcCharacterOperateError(Resp.TYPE_RIDE_NONE);
            return;
        }
        if (GTLauncher.Instance.CurrCopyType != ECopyType.TYPE_CITY &&
            GTLauncher.Instance.CurrCopyType != ECopyType.TYPE_WORLD)
        {
            CharacterHelper.CalcCharacterOperateError(Resp.TYPE_RIDE_NOTDOATSCENE);
            return;
        }
        if (GTWorld.Main.IsFSMLayer2() || GTWorld.Main.IsFSMLayer3())
        {
            CharacterHelper.CalcCharacterOperateError(Resp.TYPE_RIDE_NOTDOATFSM);
            return;
        }
        if (GTWorld.Main.IsRide)
        {
            CharacterHelper.CalcCharacterOperateError(Resp.TYPE_RIDE_ING);
            return;
        }
        if (GTWorld.Main.FSM == FSMState.FSM_SKILL)
        {
            CharacterHelper.CalcCharacterOperateError(Resp.TYPE_SKILL_CASTING);
            return;
        }
        Vector3 pos = GTWorld.Main.Pos;
        Vector3 euler = GTWorld.Main.Euler;
        GTTransform bornData = GTTransform.Create(pos, euler);
        GTWorld.Main.Mount = GTWorld.Instance.AddActor(data.Mount, EBattleCamp.CT_NONE, EActorType.MOUNT, bornData, null, false);
        GTWorld.Main.Mount.Host = GTWorld.Main;
        Resp resp = GTWorld.Main.Command.Get<CommandRideBegin>().Do();
        if (resp == Resp.TYPE_YES)
        {
            GTWorld.Main.PausedAI(true);
        }
        CharacterHelper.CalcCharacterOperateError(resp);
    }

    public void TryLeaveMount()
    {
        if (GTWorld.Main == null || GTWorld.Main.CacheTransform == null)
        {
            return;
        }
        if (GTWorld.Main.IsFSMLayer2() || GTWorld.Main.IsFSMLayer3())
        {
            return;
        }
        if (GTWorld.Main.IsRide == false)
        {
            return;
        }
        GTWorld.Instance.SetCharacterParent(GTWorld.Main);
        GTWorld.Instance.DelActor(GTWorld.Main.Mount);
        Resp resp = GTWorld.Main.Command.Get<CommandRideLeave>().Do();
        GTWorld.Main.Mount = null;
        CharacterHelper.CalcCharacterOperateError(resp);
        GTWorld.Main.PausedAI(false);
    }

    public void TryMoveJoystick(float x, float y)
    {
        if (GTWorld.Main == null || GTWorld.Main.CacheTransform == null)
        {
            return;
        }
        GTWorld.Main.PausedAI(true);
        if (GTWorld.Main.IsFSMLayer2())
        {
            return;
        }
        Camera cam = GTCameraManager.Instance.MainCamera;
        Quaternion q = new Quaternion();
        float r = Mathf.Deg2Rad * cam.transform.eulerAngles.y * 0.5f;
        q.w = Mathf.Cos(r);
        q.x = 0;
        q.y = Mathf.Sin(r);
        q.z = 0;
        Vector3 motion = q * new Vector3(x, 0, y);
        Resp resp;
        if (GTWorld.Main.IsRide)
        {
            resp = GTWorld.Main.Mount.Command.Get<CommandMove>().Update(motion).Do();
        }
        else
        {
            resp = GTWorld.Main.Command.Get<CommandMove>().Update(motion).Do();
        }
        CharacterHelper.CalcCharacterOperateError(resp);
    }

    public void TryStopJoystick()
    {
        if (GTWorld.Main == null || GTWorld.Main.CacheTransform == null)
        {
            return;
        }
        GTWorld.Main.PausedAI(false);
        if (GTWorld.Main.IsFSMLayer2())
        {
            return;
        }
        Resp resp;
        if (GTWorld.Main.IsRide)
        {
            resp = GTWorld.Main.Mount.Command.Get<CommandIdle>().Do();
        }
        else
        {
            resp = GTWorld.Main.Command.Get<CommandIdle>().Do();
        }
        CharacterHelper.CalcCharacterOperateError(resp);
        GTNetworkSend.Instance.TrySyncIdle();
    }

    public void TryPursue(Vector3 destPosition)
    {
        if (GTWorld.Main == null || GTWorld.Main.CacheTransform == null)
        {
            return;
        }
        if (GTWorld.Main.IsFSMLayer2())
        {
            return;
        }
        Resp resp;
        if (GTWorld.Main.IsRide)
        {
            resp = GTWorld.Main.Mount.Command.Get<CommandMove>().Update(destPosition, null).Do();
        }
        else
        {
            resp = GTWorld.Main.Command.Get<CommandMove>().Update(destPosition, null).Do();
        }
        CharacterHelper.CalcCharacterOperateError(resp);
    }

    public void TryJump()
    {
        if (GTWorld.Main == null || GTWorld.Main.CacheTransform == null)
        {
            return;
        }
        GTWorld.Main.PausedAI(true);
        Resp resp = GTWorld.Main.Command.Get<CommandJump>().Do();
        CharacterHelper.CalcCharacterOperateError(resp);
        GTWorld.Main.PausedAI(false);
    }

    public void TryReborn(int rebornType)
    {
        switch(rebornType)
        {
            case 0:
                GTWorld.Main.CurrAttr.HP = GTWorld.Main.CurrAttr.MaxHP;
                GTWorld.Main.CurrAttr.MP = GTWorld.Main.CurrAttr.MaxMP;
                break;
            case 1:
                GTWorld.Main.CurrAttr.HP = GTWorld.Main.CurrAttr.MaxHP / 2;
                GTWorld.Main.CurrAttr.MP = GTWorld.Main.CurrAttr.MaxMP / 2;
                break;
        }
        GTWorld.Main.Command.Get<CommandReborn>().Do();
        GTEventCenter.FireEvent(GTEventID.TYPE_UPDATE_AVATAR_HP);
        GTEventCenter.FireEvent(GTEventID.TYPE_UPDATE_AVATAR_MP);
    }

    public void TryMainCopyReq(UInt64 roleGUID, Int32 dwCopyID)
    {
        DCopy copyDB = ReadCfgCopy.GetDataById(dwCopyID);
        if (GTItemHelper.CheckItemEnongh(copyDB.CostActionId, copyDB.CostActionNum) == false)
        {
            return;
        }

        MainCopyReq req = new MainCopyReq();
        req.CopyID = dwCopyID;
        NetworkManager.Instance.Send<MainCopyReq>(MessageID.MSG_MAIN_COPY_REQ, req, roleGUID, 0);
    }

    public void TryMainCopyPass(int chapter, int copyID, int starNum)
    {

    }

    public void TryMainCopyReceiveReward(int chapter, int index)
    {
        ERewardState rewardState = MLRaid.Instance.GetChapterRewardStateByAwardIndex(chapter, index);
        switch (rewardState)
        {
            case ERewardState.NOT_RECEIVE:
                GTItemHelper.ShowTip("未达成条件");
                return;
            case ERewardState.HAS_RECEIVE:
                GTItemHelper.ShowTip("奖励已领取");
                return;
        }
        DCopyMainChapter chapterDB = ReadCfgCopyMainChapter.GetDataById(chapter);
        DAward awardDB = ReadCfgAward.GetDataById(chapterDB.Awards[index]);
        if (GTItemHelper.CheckBagFull(awardDB.MaxDropNum))
        {
            return;
        }
    }
}
