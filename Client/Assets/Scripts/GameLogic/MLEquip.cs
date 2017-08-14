using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Protocol;

public class MLEquip : GTSingleton<MLEquip>
{
    private Dictionary<int, List<int>> mEquipSuits           = new Dictionary<int, List<int>>();
    private List<XItem>                mAllStrengthenMatList = new List<XItem>();

    public string GetEquipStepByID(int equipID)
    {
        DEquip equipDB = ReadCfgEquip.GetDataById(equipID);
        switch (equipDB.Quality)
        {
            case 1:
                return "普通";
            case 2:
                return "优良";
            case 3:
                return "精良";
            case 4:
                return "史诗";
            default:
                return "传说";
        }
    }

    public string GetEquipAdvanceNameByLevel(int advanceLevel)
    {
        switch (advanceLevel)
        {
            case 1:
                return "[00ff00]初心[-]";
            case 2:
                return "[97FFFF]元婴[-]";
            case 3:
                return "[ADFF2F]离骚[-]";
            case 4:
                return "[EEEE00]空明[-]";
            case 5:
                return "[EE9A00]蛮荒[-]";
            case 6:
                return "[EE4000]无极[-]";
            case 7:
                return "[EE0000]湮灭[-]";
            case 8:
                return "[FFD700]化神[-]";
            default:
                return string.Empty;

        }
    }

    public string GetEquipTypeNameByID(int equipID)
    {
        DEquip equipDB = ReadCfgEquip.GetDataById(equipID);
        switch (equipDB.Pos)
        {
            case 1:
                return "头部";
            case 2:
                return "项链";
            case 3:
                return "衣服";
            case 4:
                return "鞋子";
            case 5:
                return "手套";
            case 6:
                return "戒指";
            case 7:
                return "法宝";
            case 8:
                return "武器";
            default:
                return "未知";
        }
    }

    public string GetEquipSuitNameByID(int equipID)
    {
        DEquip equipDB = ReadCfgEquip.GetDataById(equipID);
        DEquipSuit suitDB = ReadCfgEquipSuit.GetDataById(equipDB.Suit);
        return suitDB != null ? suitDB.Name : string.Empty;
    }

    public int GetSameSuitCountByEquipID(int equipID)
    {
        List<int> list = GetSameSuitEquipIDArrayByID(equipID);
        int count = 0;
        Dictionary<int, XItem>.Enumerator em = DataDBSDressEquip.Dict.GetEnumerator();
        while (em.MoveNext())
        {
            if (em.Current.Value.Instance == 0) continue;
            if (list.Contains(equipID)) count++;
        }
        return count;
    }

    public int GetStrengthenExpByMatList(List<XItem> list)
    {
        if (list == null) return 0;
        int exp = 0;
        for (int i = 0; i < list.Count; i++)
        {
            if (list[i].PosType == (int)EPosType.RoleMoney)
            {
                exp += list[i].Num;
            }
            else
            {
                XItem mat = list[i];
                DEquip equipDB = ReadCfgEquip.GetDataById(mat.Id);
                XEquip equip = DataDBSEquip.GetDataById(mat.Instance);
                exp += equipDB.DeComposeNum2;
                exp += equip.StrengthenExp;
                int lv = equip.StrengthenLevel;
                for (int j = lv; j > 0; j--)
                {
                    DEquipStrengLevel db = ReadCfgEquipStrengLevel.GetDataById(equipDB.Quality * 1000 + j);
                    exp += db.RequireExp;
                }
            }
        }
        return exp;
    }

    public int GetMaxEquipStrengthLevel(int equipID)
    {
        return 120;
    }

    public int GetMaxEquipAdvanceLevel(int equipID)
    {
        DEquipAdvance db = ReadCfgEquipAdvance.GetDataById(equipID);
        return db.Propertys.Count;
    }

    public int GetMaxEquipStarLevel(int equipID)
    {
        return 5;
    }

    public int CompareStrengthMat(XItem x1, XItem x2)
    {
        XEquip e1 = DataDBSEquip.GetDataById(x1.Instance);
        XEquip e2 = DataDBSEquip.GetDataById(x2.Instance);
        DEquip db1 = ReadCfgEquip.GetDataById(e1.Id);
        DEquip db2 = ReadCfgEquip.GetDataById(e2.Id);
        if (db1.Quality != db2.Quality)
        {
            return db1.Quality - db2.Quality;
        }
        if (e1.StarLevel != e2.StarLevel)
        {
            return e1.StarLevel - e2.StarLevel;
        }
        if (e1.AdvanceLevel != e2.AdvanceLevel)
        {
            return e1.AdvanceLevel - e2.AdvanceLevel;
        }
        if (e1.StrengthenExp != e2.StrengthenExp)
        {
            return e1.StrengthenExp - e2.StrengthenExp;
        }
        return x2.Pos - x1.Pos;
    }

    public bool IsFullStrengthenLevel(XEquip equip)
    {
        return equip.StrengthenLevel >= GetMaxEquipStrengthLevel(equip.Id);
    }

    public bool IsFullAdvanceLevel(XEquip equip)
    {
        return equip.AdvanceLevel >= GetMaxEquipAdvanceLevel(equip.Id);
    }

    public bool IsFullStarLevel(XEquip equip)
    {
        return equip.StarLevel > GetMaxEquipStarLevel(equip.Id);
    }

    public bool IsCanAsMatToAutoAdd(XItem item)
    {
        XEquip equip = DataDBSEquip.GetDataById(item.Instance);
        DEquip equipDB = ReadCfgEquip.GetDataById(equip.Id);
        return equipDB.Quality < 3;
    }

    public List<XItem> GetAllEquipMatList()
    {
        mAllStrengthenMatList.Clear();
        Dictionary<int, XItem>.Enumerator em = DataDBSBagItem.Dict.GetEnumerator();
        while (em.MoveNext())
        {
            XItem item = em.Current.Value; ;
            DItem itemDB = ReadCfgItem.GetDataById(item.Id);
            if (itemDB.ItemType != EItemType.EQUIP)
            {
                continue;
            }
            mAllStrengthenMatList.Add(item);
        }
        em.Dispose();
        mAllStrengthenMatList.Sort(CompareStrengthMat);
        return mAllStrengthenMatList;
    }

    public List<XItem> GetBagSameEquipList(XEquip equip)
    {
        List<XItem> list = new List<XItem>();
        Dictionary<int, XItem>.Enumerator em = DataDBSBagItem.Dict.GetEnumerator();
        while (em.MoveNext())
        {
            XItem item = em.Current.Value;
            DItem itemDB = ReadCfgItem.GetDataById(item.Id);
            if (itemDB.ItemType != EItemType.EQUIP) continue;
            XEquip e = DataDBSEquip.GetDataById(item.Instance);
            if (e.Id != equip.Id)
                continue;
            if (e.Instance == equip.Instance)
                continue;
            if (e.StarLevel > 0)
                continue;
            if (e.StrengthenLevel > 0)
                continue;
            if (e.StrengthenExp > 0)
                continue;
            if (e.AdvanceLevel > 0)
                continue;
            list.Add(item);
        }
        em.Dispose();
        return list;
    }

    public List<XItem> GetItemListToOneKeyStrengthen(XEquip equip)
    {
        List<XItem> list = new List<XItem>();
        DEquip equipDB = ReadCfgEquip.GetDataById(equip.Id);
        DEquipStrengLevel levelDB = ReadCfgEquipStrengLevel.GetDataById(equipDB.Quality * 1000 + equip.StrengthenLevel + 1);
        int moneyNum = GTDataManager.Instance.GetItemCountById(DEquip.EQUIP_STRENGTHEN_MONEY_ID_2);
        if (moneyNum > 0)
        {
            int needNum = levelDB.RequireExp - equip.StrengthenExp;
            int addNum = moneyNum >= needNum ? needNum : moneyNum;
            XItem item = new XItem();
            item.Id = DEquip.EQUIP_STRENGTHEN_MONEY_ID_2;
            item.PosType = (int)EPosType.RoleMoney;
            item.Num = addNum;
            list.Add(item);
            return list;
        }
        List<XItem> allEquipMatList = GetAllEquipMatList();
        for (int i = 0; i < list.Count; i++)
        {
            if (list.Count >= 6)
            {
                break;
            }
            if (IsCanAsMatToAutoAdd(allEquipMatList[i]))
            {
                XItem item = allEquipMatList[i];
                list.Add(item);
            }
        }
        return list;
    }

    public List<int>   GetSameSuitEquipIDArrayByID(int equipID)
    {
        DEquip equipDB = ReadCfgEquip.GetDataById(equipID);
        if (equipDB.Suit == 0)
        {
            return new List<int>();
        }
        if (mEquipSuits.ContainsKey(equipDB.Suit))
        {
            return mEquipSuits[equipDB.Suit];
        }
        Dictionary<int, DEquip>.Enumerator em = ReadCfgEquip.Dict.GetEnumerator();
        while (em.MoveNext())
        {
            List<int> list;
            int suit = em.Current.Value.Suit;
            if (mEquipSuits.ContainsKey(suit))
            {
                list = mEquipSuits[suit];
            }
            else
            {
                list = new List<int>();
                mEquipSuits.Add(suit, list);
            }
            if (!list.Contains(em.Current.Key))
            {
                list.Add(em.Current.Key);
            }
        }

        if (mEquipSuits.ContainsKey(equipDB.Suit))
        {
            return mEquipSuits[equipDB.Suit];
        }
        else
        {
            return new List<int>();
        }

    }

}
