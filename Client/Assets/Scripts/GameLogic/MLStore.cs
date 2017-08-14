using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;

public class MLStore : GTSingleton<MLStore> , IModule
{
    public Dictionary<int, List<DStore>> RandomProducts = new Dictionary<int, List<DStore>>();

    public void Clear()
    {
        RandomProducts.Clear();
    }

    public List<DStore> GetSelectProductListByStoreType(int storeType)
    {
        List<DStore> list = null;
        RandomProducts.TryGetValue(storeType, out list);
        if (list != null)
        {
            return list;
        }
        DStoreType db = ReadCfgStoreType.GetDataById(storeType);
        list = GetProductListByStoreType(storeType);
        list = GetProductListByCurRole(list);
        if (db.Random)
        {
            list = GTTools.RandomListFromBigList<DStore>(list, db.Limit);
        }
        RandomProducts[storeType] = list;
        return list;
    }

    List<DStore> GetProductListByCurRole(List<DStore> pList)
    {
        List<DStore> list = new List<DStore>();
        DRole db = ReadCfgRole.GetDataById(GTGlobal.CurPlayerCarrerID);
        int carrer = db.Carrer;
        for (int i = 0; i < pList.Count; i++)
        {
            DItem itemDB = ReadCfgItem.GetDataById(pList[i].ItemID);
            if (itemDB.Carrer == carrer || itemDB.Carrer == 0)
            {
                list.Add(pList[i]);
            }
        }
        return list;
    }

    List<DStore> GetProductListByStoreType(int storeType)
    {
        List<DStore> list = new List<DStore>();
        foreach (var current in ReadCfgStore.Dict)
        {
            if (current.Value.StoreType == storeType)
            {
                list.Add(current.Value);
            }
        }
        return list;
    }
}
