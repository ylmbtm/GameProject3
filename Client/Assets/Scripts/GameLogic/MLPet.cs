using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Protocol;

public class MLPet : GTSingleton<MLPet>
{
    private List<int> mUpItemList = new List<int>()
    {
        DPetLevel.PET_UP_ITEM_ID_1,
        DPetLevel.PET_UP_ITEM_ID_2,
        DPetLevel.PET_UP_ITEM_ID_3,
    };

    public bool      IsFood(int id)
    {
        return mUpItemList.Contains(id);
    }

    public int       GetExpByItemList(List<XItem> list)
    {
        int exp = 0;
        if (list == null) return 0;
        for (int i = 0; i < list.Count; i++)
        {
            XItem tab = list[i];
            if (IsFood(tab.Id))
            {
                exp += ReadCfgItem.GetDataById(tab.Id).Data1;
            }
        }
        return exp;
    }

    public void      GetItemListToOneKeyUpgrade(ref List<XItem> list)
    {
        if (list == null)
        {
            return;
        }
        list.Clear();
        for (int i = 0; i < mUpItemList.Count; i++)
        {
            int count = GTDataManager.Instance.GetItemCountById(mUpItemList[i]);
            for (int k = 0; k < count; k++)
            {
                if (list.Count >= 6)
                {
                    return;
                }
                else
                {
                    XItem item = new XItem();
                    item.Id = mUpItemList[i];
                    item.Num = 1;
                    list.Add(item);
                }
            }
        }
    }

    public List<int> GetUpGradeItemIDList()
    {
        return mUpItemList;
    }
}
