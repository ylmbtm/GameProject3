using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Protocol;

public class MLGem : GTSingleton<MLGem>
{
    private Dictionary<int, List<int>> mGemSuits;
    private HashSet<int>               mStrengHashSet;
    private Dictionary<int, int>       mGemIndexDict;

    public MLGem()
    {
        mGemSuits = new Dictionary<int, List<int>>();
        mStrengHashSet = new HashSet<int>()
        {
            DGemLevel.GEM_STRENGTHEN_ITEM_ID_1,
            DGemLevel.GEM_STRENGTHEN_ITEM_ID_2,
            DGemLevel.GEM_STRENGTHEN_ITEM_ID_3,
            DGemLevel.GEM_STRENGTHEN_ITEM_ID_4,
            DGemLevel.GEM_STRENGTHEN_ITEM_ID_5,
        };
        mGemIndexDict = new Dictionary<int, int>();
        for (int i = 1; i <= 40; i++)
        {
            int value = (i % 5 != 0) ? (i + 5 - i % 5) / 5 : i / 5;
            mGemIndexDict.Add(i, value);
        }
    }

    public bool IsFullStrengthenLevel(XGem gem)
    {
        return gem.StrengthenLevel >= 120;
    }

    public bool IsCanBeUseAsStrengthenMatToAutoAdd(XItem item)
    {
        XGem gem = DataDBSGem.GetDataById(item.Instance);
        DGem gemDB = ReadCfgGem.GetDataById(gem.Id);
        return gemDB.Quality < 3;
    }

    public string GetGemTypeName(int itemID)
    {
        DGem db = ReadCfgGem.GetDataById(itemID);
        switch (db.Pos)
        {
            case 1:
                return "金";
            case 2:
                return "木";
            case 3:
                return "水";
            case 4:
                return "火";
            case 5:
                return "土";
            default:
                return "空";
        }
    }

    public string GetGemSuitName(int itemID)
    {
        DGem gemDB = ReadCfgGem.GetDataById(itemID);
        if (gemDB.Suit == 0)
        {
            return string.Empty;
        }
        return ReadCfgGemSuit.GetDataById(gemDB.Suit).Name;
    }

    public List<int> GetSameSuitIDListByID(int itemID)
    {
        DGem gemDB = ReadCfgGem.GetDataById(itemID);

        if (gemDB.Suit == 0)
        {
            return new List<int>();
        }
        if (mGemSuits.ContainsKey(gemDB.Suit))
        {
            return mGemSuits[gemDB.Suit];
        }
        Dictionary<int, DGem>.Enumerator em = ReadCfgGem.Dict.GetEnumerator();
        while (em.MoveNext())
        {
            DGem db = em.Current.Value;
            List<int> list;
            if (mGemSuits.ContainsKey(db.Suit))
            {
                list = mGemSuits[db.Suit];
            }
            else
            {
                list = new List<int>();
                mGemSuits.Add(db.Suit, list);
            }
            if (!list.Contains(db.Id))
            {
                list.Add(db.Id);
            }
        }
        if (mGemSuits.ContainsKey(gemDB.Suit))
        {
            return mGemSuits[gemDB.Suit];
        }
        else
        {
            return new List<int>();
        }

    }

    public int GetActiveSameSuitsCountByPos(int pos)
    {
        if (!DataDBSDressGem.ContainsKey(pos))
            return 0;
        XItem dressGem = DataDBSDressGem.GetDataById(pos);
        if (!DataDBSGem.ContainsKey(dressGem.Instance))
            return 0;
        XGem gem = DataDBSGem.GetDataById(dressGem.Instance);
        int p = (int)(pos / 5.1f);
        List<int> sameSuits = GetSameSuitIDListByID(gem.Id);
        int count = 0;
        for (int i = 0; i < sameSuits.Count; i++)
        {
            int suitPos = p * 5 + i + 1;
            if (!DataDBSDressGem.ContainsKey(suitPos))
            {
                continue;
            }
            XItem item = DataDBSDressGem.GetDataById(suitPos);
            if (!DataDBSGem.ContainsKey(item.Instance))
            {
                continue;
            }
            XGem g = DataDBSGem.GetDataById(item.Instance);
            if (sameSuits.Contains(g.Id))
            {
                count++;
            }
        }
        return count;

    }

    public int GetDressGemPosByIndexAndTargetPos(int index, int pos)
    {
        return (index - 1) * 5 + pos;
    }

    public int GetIndexByDressGemPos(int pos)
    {
        if (mGemIndexDict.ContainsKey(pos))
            return mGemIndexDict[pos];
        return 0;
    }

    public int CompareXGem(XItem x1, XItem x2)
    {
        XGem e1 = DataDBSGem.GetDataById(x1.Instance);
        XGem e2 = DataDBSGem.GetDataById(x2.Instance);
        DGem db1 = ReadCfgGem.GetDataById(e1.Id);
        DGem db2 = ReadCfgGem.GetDataById(e2.Id);
        if (db1.Quality != db2.Quality)
        {
            return db1.Quality - db2.Quality;
        }
        if (e1.StrengthenLevel != e2.StrengthenLevel)
        {
            return e1.StrengthenLevel - e2.StrengthenLevel;
        }
        if (e1.StrengthenExp != e2.StrengthenExp)
        {
            return e1.StrengthenExp - e2.StrengthenExp;
        }
        return x2.Pos - x1.Pos;
    }

    public List<XItem> GetAllGemList(XGem gem)
    {
        List<XItem> list = new List<XItem>();
        Dictionary<int, XItem>.Enumerator em = DataDBSBagGem.Dict.GetEnumerator();
        while (em.MoveNext())
        {
            if (em.Current.Value.Instance == gem.Instance) continue;
            XItem item = em.Current.Value;
            list.Add(item);
        }
        em.Dispose();
        list.Sort(CompareXGem);
        return list;
    }

    public List<XItem> GetItemListToOneKeyStrengthen(XGem gem)
    {
        List<XItem> list = new List<XItem>();
        Dictionary<int, XItem> bagItems = GTDataManager.Instance.GetItemDataByPosType(EPosType.BagItem);
        HashSet<int>.Enumerator em = mStrengHashSet.GetEnumerator();
        while (em.MoveNext())
        {
            int id = em.Current;
            XItem data = GTDataManager.Instance.GetItemDataById(id);
            if (data == null || list.Count > 0)
            {
                continue;
            }
            int num = data.Num;
            for (int i = 0; i < data.Num; i++)
            {
                if (num == 0 || list.Count >= 6)
                {
                    break;
                }
                else
                {
                    XItem newData = new XItem();
                    newData.Id = data.Id;
                    newData.Instance = data.Instance;
                    newData.Pos = data.Pos;
                    newData.PosType = data.PosType;
                    newData.Num = 1;
                    list.Add(newData);
                    num--;
                }
            }
        }

        List<XItem> pGemList = GetAllGemList(gem);
        for (int i = 0; i < pGemList.Count; i++)
        {
            if (list.Count >= 6) break;
            XItem item = pGemList[i];
            list.Add(item);
        }
        return list;
    }

    public int GetExpByStrengthMatList(List<XItem> list)
    {
        int value = 0;
        for (int i = 0; i < list.Count; i++)
        {
            XItem item = list[i];
            switch ((EPosType)item.PosType)
            {
                case EPosType.BagItem:
                    DItem itemDB = ReadCfgItem.GetDataById(item.Id);
                    value += itemDB.Data1 * item.Num;
                    break;
                case EPosType.BagGem:
                    XGem gem = DataDBSGem.GetDataById(item.Instance);
                    value += GetExpByXGem(gem);
                    break;
            }
        }
        return value;
    }

    public int GetExpByXGem(XGem gem)
    {
        DGem gemDB = ReadCfgGem.GetDataById(gem.Id);
        int exp = gem.StrengthenExp + gemDB.Exp;
        int lv = gem.StrengthenLevel;
        for (int i = 0; i < lv; i++)
        {
            DGemLevel db = ReadCfgGemLevel.GetDataById(gemDB.Quality * 1000 + gem.StrengthenLevel);
            exp += db.RequireExp;
        }
        return exp;
    }

}
