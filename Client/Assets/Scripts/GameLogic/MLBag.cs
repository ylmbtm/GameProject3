using UnityEngine;
using System.Collections;
using Protocol;

public class MLBag : GTSingleton<MLBag>
{
    public int CompareBagItem(XItem x1, XItem x2)
    {
        DItem itemDB1 = ReadCfgItem.GetDataById(x1.Id);
        DItem itemDB2 = ReadCfgItem.GetDataById(x2.Id);
        if (itemDB1.ItemType != itemDB2.ItemType)
        {
            return itemDB1.ItemType - itemDB2.ItemType;
        }
        if (itemDB1.Quality != itemDB2.Quality)
        {
            return itemDB2.Quality - itemDB1.Quality;
        }
        if (itemDB1.ItemType == EItemType.EQUIP)
        {
            XEquip xe1 = DataDBSEquip.GetDataById(x1.Instance);
            XEquip xe2 = DataDBSEquip.GetDataById(x2.Instance);
            return CompareBagEquip(xe1, xe2);
        }
        if (itemDB1.Id != itemDB2.Id)
        {
            return itemDB2.Id - itemDB1.Id;
        }
        return x1.Instance - x2.Instance;
    }

    public int CompareBagEquip(XEquip xe1, XEquip xe2)
    {
        DEquip equipDB1 = ReadCfgEquip.GetDataById(xe1.Id);
        DEquip equipDB2 = ReadCfgEquip.GetDataById(xe2.Id);
        if (equipDB1.Pos != equipDB2.Pos)
        {
            return equipDB2.Pos - equipDB1.Pos;
        }
        if (xe1.StrengthenLevel != xe2.StarLevel)
        {
            return xe2.StrengthenLevel - xe1.StrengthenLevel;
        }
        if (xe1.AdvanceLevel != xe2.AdvanceLevel)
        {
            return xe2.AdvanceLevel - xe2.AdvanceLevel;
        }
        if (xe1.StarLevel != xe2.StarLevel)
        {
            return xe2.StarLevel - xe1.StarLevel;
        }
        if (equipDB1.Id != equipDB2.Id)
        {
            return equipDB2.Id - equipDB1.Id;
        }
        return xe1.Instance - xe2.Instance;

    }

    public int CompareBagGem(XItem x1, XItem x2)
    {
        DItem itemDB1 = ReadCfgItem.GetDataById(x1.Id);
        DItem itemDB2 = ReadCfgItem.GetDataById(x2.Id);
        if (itemDB1.Quality != itemDB2.Quality)
        {
            return itemDB2.Quality - itemDB1.Quality;
        }
        if (itemDB1.Id != itemDB2.Id)
        {
            return itemDB1.Id - itemDB2.Id;
        }
        XGem gem1 = DataDBSGem.GetDataById(x1.Instance);
        XGem gem2 = DataDBSGem.GetDataById(x2.Instance);

        if (gem1.StrengthenLevel != gem2.StrengthenLevel)
        {
            return gem1.StrengthenLevel - gem2.StrengthenLevel;
        }
        return x1.Instance - x2.Instance;
    }

    public int CompareBagChip(XItem x1, XItem x2)
    {
        DItem itemDB1 = ReadCfgItem.GetDataById(x1.Id);
        DItem itemDB2 = ReadCfgItem.GetDataById(x2.Id);
        DItem itemTargetDB1 = ReadCfgItem.GetDataById(itemDB1.Data2);
        DItem itemTargetDB2 = ReadCfgItem.GetDataById(itemDB2.Data2);

        if (itemTargetDB1.ItemType != itemTargetDB2.ItemType)
        {
            return itemTargetDB1.ItemType - itemTargetDB2.ItemType;
        }
        if (itemDB1.Quality != itemDB2.Quality)
        {
            return itemDB2.Quality - itemDB1.Quality;
        }
        if (x1.Num != x2.Num)
        {
            return x2.Num - x1.Num;
        }
        return x2.Id - x1.Id;
    }

    public int CompareBagFashion(XItem x1, XItem x2)
    {
        DItem itemDB1 = ReadCfgItem.GetDataById(x1.Id);
        DItem itemDB2 = ReadCfgItem.GetDataById(x2.Id);
        if (itemDB1.Quality != itemDB2.Quality)
        {
            return itemDB2.Quality - itemDB1.Quality;
        }
        if (itemDB1.Id != itemDB2.Id)
        {
            return itemDB2.Id - itemDB1.Id;
        }
        return x1.Instance - x2.Instance;
    }

    public int CompareBagRune(XItem x1, XItem x2)
    {
        DItem itemDB1 = ReadCfgItem.GetDataById(x1.Id);
        DItem itemDB2 = ReadCfgItem.GetDataById(x2.Id);
        if (itemDB1.Quality != itemDB2.Quality)
        {
            return itemDB2.Quality - itemDB1.Quality;
        }

        if (itemDB1.Id != itemDB2.Id)
        {
            return itemDB2.Id - itemDB1.Id;
        }
        XRune r1 = DataDBSRune.GetDataById(x1.Instance);
        XRune r2 = DataDBSRune.GetDataById(x2.Instance);
        if (r1.Level != r2.Level)
        {
            return r2.Level - r1.Level;
        }
        return x1.Instance - x2.Instance;
    }
}
