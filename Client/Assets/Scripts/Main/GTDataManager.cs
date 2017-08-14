using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using Protocol;

public class GTDataManager : GTSingleton<GTDataManager>
{
    private int mCurRoleID;

    string GetDataPath(string xmlName)
    {
        string pPath = string.Format("{0}/Data/{1}", GTResourceManager.Instance.GetExtPath(), mCurRoleID);
        if (!Directory.Exists(pPath))
        {
            Directory.CreateDirectory(pPath);
        }
        string path = string.Format("{0}/{1}", pPath, xmlName);
        return path;
    }

    string GetCommonDataPath(string xmlName)
    {
        string pPath = string.Format("{0}/Data", GTResourceManager.Instance.GetExtPath());
        if (!Directory.Exists(pPath))
        {
            Directory.CreateDirectory(pPath);
        }
        string path = string.Format("{0}/{1}", pPath, xmlName);
        return path;
    }

    public void   LoadCommonData()
    {
        DataDBSCharacter.Read(GetCommonDataPath(GTDataKey.Data_Roles), EDataKeyType.Carrer);
    }

    public void   LoadRoleData(int id)
    {
        this.mCurRoleID = id;
        this.DumpRoleData();
        this.ReadRoleData();
    }

    void   DumpRoleData()
    {
        DataDBSMoney.Clear();
        DataDBSAction.Clear();
        DataDBSSoul.Clear();

        DataDBSBagItem.Clear();
        DataDBSBagGem.Clear();
        DataDBSBagChip.Clear();
        DataDBSBagRune.Clear();
        DataDBSBagFashion.Clear();

        DataDBSDressEquip.Clear();
        DataDBSDressFashion.Clear();
        DataDBSDressGem.Clear();
        DataDBSDressRune.Clear();

        DataDBSEquip.Clear();
        DataDBSGem.Clear();
        DataDBSRune.Clear();
        DataDBSMainChapter.Clear();
        DataDBSCopy.Clear();
        DataDBSRaid.Clear();

        DataDBSRelics.Clear();
        DataDBSMount.Clear();
        DataDBSPartner.Clear();
        DataDBSPet.Clear();
        DataDBSGuide.Clear();

        DataDBSThreadTask.Clear();
        DataDBSBranchTask.Clear();
        DataDBSDailyTask.Clear();
    }

    void   ReadRoleData()
    {
        DataDBSBagItem.     Read(GetDataPath(GTDataKey.Data_BagItems),       EDataKeyType.Pos);
        DataDBSEquip.       Read(GetDataPath(GTDataKey.Data_Equips),         EDataKeyType.Instance);
        DataDBSDressEquip.  Read(GetDataPath(GTDataKey.Data_DressEquips),    EDataKeyType.Pos);
        DataDBSMoney.       Read(GetDataPath(GTDataKey.Data_Moneys),         EDataKeyType.Id);
        DataDBSAction.      Read(GetDataPath(GTDataKey.Data_Actions),        EDataKeyType.Id);
        DataDBSSoul.        Read(GetDataPath(GTDataKey.Data_Souls),          EDataKeyType.Id);
        DataDBSBagGem.      Read(GetDataPath(GTDataKey.Data_BagGems),        EDataKeyType.Pos);
        DataDBSGem.         Read(GetDataPath(GTDataKey.Data_Gems),           EDataKeyType.Instance);
        DataDBSDressGem.    Read(GetDataPath(GTDataKey.Data_DressGems),      EDataKeyType.Pos);
        DataDBSBagChip.     Read(GetDataPath(GTDataKey.Data_BagChips),       EDataKeyType.Pos);
        DataDBSBagFashion.  Read(GetDataPath(GTDataKey.Data_BagFashions),    EDataKeyType.Pos);
        DataDBSBagRune.     Read(GetDataPath(GTDataKey.Data_BagRunes),       EDataKeyType.Pos);
        DataDBSDressRune.   Read(GetDataPath(GTDataKey.Data_DressRunes),     EDataKeyType.Pos);
        DataDBSDressFashion.Read(GetDataPath(GTDataKey.Data_DressFashions),  EDataKeyType.Pos);
        DataDBSRune.        Read(GetDataPath(GTDataKey.Data_Runes),          EDataKeyType.Instance);
        DataDBSCopy.        Read(GetDataPath(GTDataKey.Data_Copys),          EDataKeyType.Id);
        DataDBSMainChapter. Read(GetDataPath(GTDataKey.Data_MainChapters),   EDataKeyType.Id);
        DataDBSRaid.        Read(GetDataPath(GTDataKey.Data_Raid),           EDataKeyType.Id);
        DataDBSPartner.     Read(GetDataPath(GTDataKey.Data_Partners),       EDataKeyType.Id);
        DataDBSMount.       Read(GetDataPath(GTDataKey.Data_Mounts),         EDataKeyType.Id);
        DataDBSRelics.      Read(GetDataPath(GTDataKey.Data_Relics),         EDataKeyType.Id);
        DataDBSPet.         Read(GetDataPath(GTDataKey.Data_Pets),           EDataKeyType.Id);
        DataDBSGuide.       Read(GetDataPath(GTDataKey.Data_Guides),         EDataKeyType.Id);
        DataDBSThreadTask.  Read(GetDataPath(GTDataKey.Data_MainTasks),      EDataKeyType.Id);
        DataDBSBranchTask.  Read(GetDataPath(GTDataKey.Data_BranchTasks),    EDataKeyType.Id);
        DataDBSDailyTask.   Read(GetDataPath(GTDataKey.Data_DailyTasks),     EDataKeyType.Id);
    }

    public Dictionary<int, XItem> GetItemDataByPosType(EPosType posType)
    {
        switch (posType)
        {
            case EPosType.BagItem:
                return DataDBSBagItem.Dict;
            case EPosType.BagChip:
                return DataDBSBagChip.Dict;
            case EPosType.BagFashion:
                return DataDBSBagFashion.Dict;
            case EPosType.BagGem:
                return DataDBSBagGem.Dict;
            case EPosType.BagRune:
                return DataDBSBagRune.Dict;
            case EPosType.RoleEquip:
                return DataDBSDressEquip.Dict;
            case EPosType.RoleFashion:
                return DataDBSDressFashion.Dict;
            case EPosType.RoleGem:
                return DataDBSDressGem.Dict;
            case EPosType.RoleRune:
                return DataDBSDressRune.Dict;
            default:
                return new Dictionary<int, XItem>();
        }
    }

    public Dictionary<int, XItem> GetBagDataByBagType(EBagType bagType)
    {
        switch (bagType)
        {
            case EBagType.ITEM:
                return DataDBSBagItem.Dict;
            case EBagType.CHIP:
                return DataDBSBagChip.Dict;
            case EBagType.FASHION:
                return DataDBSBagFashion.Dict;
            case EBagType.GEM:
                return DataDBSBagGem.Dict;
            case EBagType.RUNE:
                return DataDBSBagRune.Dict;
            default:
                return new Dictionary<int, XItem>();
        }
    }

    public XItem  GetItemDataById(int id)
    {
        DItem itemDB = ReadCfgItem.GetDataById(id);
        Dictionary<int, XItem> items = GetBagDataByBagType(itemDB.BagType);
        XItem item = null;
        foreach (KeyValuePair<int, XItem> pair in items)
        {
            if (pair.Value.Id == id)
            {
                item = pair.Value;
            }
        }
        return item;
    }

    public int    GetItemCountById(int id)
    {
        DItem db = ReadCfgItem.GetDataById(id);
        if (db == null) return 0;
        int value = 0;
        switch (db.ItemType)
        {
            case EItemType.MONEY:
                {
                    XMoney data = DataDBSMoney.GetDataById(id);
                    value = data == null ? 0 : data.Num;
                }
                break;
            case EItemType.ACTION:
                {
                    XAction data = DataDBSAction.GetDataById(id);
                    value = data == null ? 0 : data.Num;
                }
                break;
            case EItemType.PETSOUL:
                {
                    XSoul data = DataDBSSoul.GetDataById(id);
                    value = data == null ? 0 : data.Num;
                }
                break;
            case EItemType.BOX:
            case EItemType.KEY:
            case EItemType.DRUG:
            case EItemType.MAT:
                foreach (KeyValuePair<int, XItem> pair in DataDBSBagItem.Dict)
                {
                    if (pair.Value.Id == id)
                    {
                        value = pair.Value.Num;
                    }
                }
                break;
            case EItemType.CHIP:
                foreach (KeyValuePair<int, XItem> pair in DataDBSBagChip.Dict)
                {
                    if (pair.Value.Id == id)
                    {
                        value = pair.Value.Num;
                    }
                }
                break;
            default:
                break;
        }
        return value;
    }

    public int    GetActionCountByType(EAction actionType)
    {
        switch (actionType)
        {
            case EAction.Manual:
                return GetItemCountById(101);
            case EAction.Energy:
                return GetItemCountById(102);
            default:
                return 0;
        }
    }

    public XEquip GetEquipDataByPos(EPosType posType, int pos)
    {
        Dictionary<int, XItem> dict = GetItemDataByPosType(posType);
        XItem item = null;
        dict.TryGetValue(pos, out item);
        if(item==null)
        {
            return null;
        }
        XEquip equip = null;
        DataDBSEquip.Dict.TryGetValue(item.Instance, out equip);
        return equip;
    }

    public XGem   GetGemDataByPos(EPosType posType, int pos)
    {
        Dictionary<int, XItem> dict = GetItemDataByPosType(posType);
        XItem item = null;
        dict.TryGetValue(pos, out item);
        if (item == null)
        {
            return null;
        }
        XGem gem = null;
        DataDBSGem.Dict.TryGetValue(item.Instance, out gem);
        return gem;
    }

    public int    GetBagNum()
    {
        return 120;
    }

    public int    MaxInstance
    {
        get { return GTGlobal.LocalData.MaxInstance; }
        set { GTGlobal.LocalData.MaxInstance = value; }
    }

    public int    GetNewPos(EBagType bagType)
    {
        Dictionary<int, XItem> itemData = GetBagDataByBagType(bagType);
        if (itemData == null) return 0;
        int newPos = 0;
        for (int i = 1; i <= GetBagNum(); i++)
        {
            if (!itemData.ContainsKey(i))
            {
                newPos = i;
                break;
            }
        }
        return newPos;
    }

    public void AddNewItem(int id, int num)
    {
        if (num < 1)
        {
            return;
        }
        DItem db = ReadCfgItem.GetDataById(id);
        if (db == null)
        {
            return;
        }
        switch (db.ItemType)
        {
            case EItemType.EQUIP:
                for (int i = 0; i < num; i++)
                {
                    MaxInstance++;
                    AddNewEquip(MaxInstance, id);
                }
                break;
            case EItemType.GEM:
                for (int i = 0; i < num; i++)
                {
                    MaxInstance++;
                    AddNewGem(MaxInstance, id);
                }
                break;
            case EItemType.MONEY:
                AddMoney(id, num);
                break;
            case EItemType.ACTION:
                AddAction(id, num);
                break;
            case EItemType.PETSOUL:
                AddSoul(id, num);
                break;
            case EItemType.FASHION:
                for (int i = 0; i < num; i++)
                {
                    this.MaxInstance++;
                    AddNewFashion(this.MaxInstance, id);
                }
                break;
            case EItemType.CHIP:
                AddChip(id, num);
                break;
            case EItemType.EXP:
                break;
            case EItemType.RUNE:
                for (int i = 0; i < num; i++)
                {
                    this.MaxInstance++;
                    AddNewRune(Instance.MaxInstance, id);
                }
                break;
            case EItemType.TASK:
            case EItemType.BOX:
            case EItemType.KEY:
            case EItemType.DRUG:
            case EItemType.MAT:
                AddItem(id, num);
                break;

        }
    }

    public void AddNewItemList(List<GTItem> list,bool showAward=true)
    {
        if(list==null)
        {
            return;
        }
        for(int i=0;i<list.Count;i++)
        {
            AddNewItem(list[i].Id, list[i].Num);
        }
        if(showAward)
        {
            GTItemHelper.ShowAwardTip(list);
        }
    }

    public void AddItem(int id, int num)
    {
        XItem item = Instance.GetItemDataById(id);
        if (item == null)
        {
            int newPos = GetNewPos(EBagType.ITEM);
            if (newPos != 0)
            {
                Instance.MaxInstance++;
                item = new XItem();
                item.Instance = this.MaxInstance;
                item.Pos = newPos;
                item.Id = id;
                item.Num = num;
                item.PosType = (int)EPosType.BagItem;
                DataDBSBagItem.Insert(newPos, item);
            }
        }
        else
        {
            item.Num += num;
            DataDBSBagItem.Update(item.Pos, item);
        }
    }

    public void AddNewEquip(int instance, int id)
    {
        int newPos = GetNewPos(EBagType.ITEM);
        if (newPos == 0)
        {
            return;
        }
        XItem item = new XItem();
        item.Instance = instance;
        item.Pos = newPos;
        item.Id = id;
        item.Num = 1;
        item.PosType = (int)EPosType.BagItem;

        DataDBSBagItem.Insert(newPos, item);
        DataDBSEquip.Update(instance, GTDataFactory.CreateEquip(id, instance));
    }

    public void AddNewGem(int instance, int id)
    {
        int newPos = GetNewPos(EBagType.GEM);
        if (newPos == 0)
        {
            return;
        }
        XItem item = new XItem();
        item.Instance = instance;
        item.Pos = newPos;
        item.Id = id;
        item.Num = 1;
        item.PosType = (int)EPosType.BagGem;

        DataDBSBagGem.Insert(newPos, item);
        DataDBSGem.Insert(instance, GTDataFactory.CreateGem(id, instance));
    }

    public void AddChip(int id, int num)
    {
        XItem item = GTDataManager.Instance.GetItemDataById(id);
        if (item == null)
        {
            int newPos = GetNewPos(EBagType.CHIP);
            if (newPos != 0)
            {
                this.MaxInstance++;
                item = new XItem();
                item.Instance = this.MaxInstance;
                item.Pos = newPos;
                item.Id = id;
                item.Num = num;
                item.PosType = (int)EPosType.BagChip;
                DataDBSBagChip.Insert(newPos, item);
            }
        }
        else
        {
            item.Num += num;
            DataDBSBagChip.Update(item.Pos, item);
        }
    }

    public void AddNewRune(int instance, int id)
    {
        int newPos = GetNewPos(EBagType.RUNE);
        if (newPos == 0)
        {
            return;
        }
        XItem item = new XItem();
        item.Pos = newPos;
        item.Instance = instance;
        item.Pos = newPos;
        item.Id = id;
        item.Num = 1;
        item.PosType = (int)EPosType.BagRune;
        DataDBSBagRune.Insert(newPos, item);

        DataDBSRune.Insert(instance, GTDataFactory.CreateRune(id, instance));
    }

    public void AddNewFashion(int instanceId, int id)
    {
        int newPos = GetNewPos(EBagType.FASHION);
        if (newPos == 0)
        {
            return;
        }
        XItem item = new XItem();
        item.Pos = newPos;
        item.Instance = instanceId;
        item.Id = id;
        item.Num = 1;
        item.PosType = (int)EPosType.BagFashion;
        DataDBSBagFashion.Insert(newPos, item);
    }

    public void AddMoney(int id, int num)
    {
        XMoney money= DataDBSMoney.GetDataById(id);
        if (money!=null)
        {
            money.Num += num;
            DataDBSMoney.Update(id, money);
        }
        else
        {
            money = new XMoney();
            money.Id = id;
            money.Num = num;
            DataDBSMoney.Insert(id, money);
        }
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_MONEY, id);
    }

    public void AddAction(int id, int num)
    {
        XAction action = DataDBSAction.GetDataById(id);
        if (action != null)
        {
            action.Num += num;
            DataDBSAction.Update(id, action);
        }
        else
        {
            action = new XAction();
            action.Id = id;
            action.Num = num;
            DataDBSAction.Insert(id, action);
        }
        DItem db   = ReadCfgItem.GetDataById(id);
        EAction type = (EAction)(id - 100);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_ACTION,type);
    }

    public void AddSoul(int id,int num)
    {
        XSoul soul = DataDBSSoul.GetDataById(id);
        if(soul==null)
        {
            soul = new XSoul();
            soul.Id = id;
            soul.Num = num;
            DataDBSSoul.Insert(id, soul);
        }
        else
        {
            soul.Num += num;
            DataDBSSoul.Update(id, soul);
        }
    }

    public bool UseItemById(int id, int num = 1)
    {
        DItem itemDB = ReadCfgItem.GetDataById(id);
        switch (itemDB.ItemType)
        {
            case EItemType.MONEY:
                return UseMoney(id, num);
            case EItemType.ACTION:
                return UseAction(id, num);
            case EItemType.PETSOUL:
                return UseSoul(id, num);
            case EItemType.BOX:
            case EItemType.DRUG:
            case EItemType.KEY:
            case EItemType.MAT:
                return UseBagItem(id, num);
            case EItemType.EXP:
                return false;
            case EItemType.CHIP:
                return UseBagChip(id, num);
            default:
                return false;
        }


    }

    public bool UseMoney(int id, int num)
    {
        XMoney money = DataDBSMoney.GetDataById(id);
        if (money == null)
        {
            return false;
        }
        if (money.Num < num)
        {
            return false;
        }
        money.Num -= num;
        DataDBSMoney.Update(id, money);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_MONEY,id);

        return true;
    }

    public bool UseAction(int id, int num)
    {
        XAction action = DataDBSAction.GetDataById(id);
        if (action == null)
        {
            return false;
        }
        if (action.Num < num)
        {
            return false;
        }
        action.Num -= num;
        DataDBSAction.Update(id, action);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_ACTION, (EAction)(id - 100));
        return true;
    }

    public bool UseSoul(int id, int num)
    {
        XSoul soul = DataDBSSoul.GetDataById(id);
        if (soul == null)
        {
            return false;
        }
        if (soul.Num < num)
        {
            return false;
        }
        soul.Num -= num;
        DataDBSSoul.Update(id, soul);
        return true;
    }

    public bool UseBagItem(int id, int num)
    {
        XItem item = GetItemDataById(id);
        if (item == null)
        {
            return false;
        }
        if (item.Num < num)
        {
            return false;
        }
        item.Num -= num;
        if (item.Num < 1)
        {
            int pos = item.Pos;
            DataDBSBagItem.Delete(pos);
        }
        else
        {
            DataDBSBagItem.Update(item.Pos, item);
        }
        return true;
    }

    public bool UseBagChip(int id, int num)
    {
        XItem item = GTDataManager.Instance.GetItemDataById(id);
        if (item == null)
        {
            return false;
        }
        if (item.Num < num)
        {
            return false;
        }
        item.Num -= num;
        if (item.Num < 1)
        {
            int pos = item.Pos;
            DataDBSBagChip.Delete(pos);
        }
        else
        {
            DataDBSBagChip.Update(item.Pos, item);
        }

        return true;
    }

    public bool DelBagEquip(int pos)
    {
        XItem item = DataDBSBagItem.GetDataById(pos);
        if (item == null)
        {
            return false;
        }
        DataDBSBagItem.Delete(pos);
        DataDBSEquip.Delete(item.Instance);
        return true;
    }

    public bool DelBagGem(int pos)
    {
        XItem item = DataDBSBagGem.GetDataById(pos);
        if(item==null)
        {
            return false;
        }
        DataDBSBagGem.Delete(pos);
        DataDBSGem.Delete(item.Instance);
        return true;
    }

    public bool DelBagFashion(int pos)
    {
        XItem item = DataDBSBagFashion.GetDataById(pos);
        if (item == null) return false;
        DataDBSBagFashion.Delete(item.Pos);
        return true;
    }

    public bool DelBagRune(int pos)
    {
        XItem item = DataDBSBagRune.GetDataById(pos);
        if (item == null) return false;
        DataDBSBagRune.Delete(item.Pos);
        DataDBSRune.Delete(item.Instance);
        return true;
    }
}