using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class AwardModule : GTSingleton<AwardModule>
{
    public List<KStruct> GetAwardDataByID(int awardID)
    {
        List<KStruct> list = new List<KStruct>();
        DAward db = ReadCfgAward.GetDataById(awardID);
        if (db == null)
        {
            return null;
        }
        switch (db.DropType)
        {
            case EDropType.MULFIX:
                CalcMulFixItem(ref list, db.DropItems);
                break;
            case EDropType.ONERAN:
                CalcOneRanItem(ref list, db.DropItems);
                break;
            case EDropType.MULRAN:
                CalcMulFixItem(ref list, db.DropItems);
                break;
        }
        return list;
    }

    void CalcOneFixItem(ref List<KStruct> list, string s)
    {
        string[] array = s.Split('|');
        KStruct m = new KStruct(array[0].ToInt32(), array[1].ToInt32());
        list.Add(m);
    }

    void CalcMulFixItem(ref List<KStruct> list, string s)
    {
        string str = s.TrimStart('(').TrimEnd(')').Replace(")(", "#");
        string[] array = str.Split('#');
        for (int i = 0; i < array.Length; i++)
        {
            string[] a = array[i].Split('|');
            KStruct m = new KStruct(a[0].ToInt32(), a[1].ToInt32());
            list.Add(m);
        }
    }

    void CalcOneRanItem(ref List<KStruct> list, string s)
    {
        string[] idValue = s.Split('&');
        KStruct m = new KStruct();
        int random = UnityEngine.Random.Range(1, 101);
        string str = s.TrimStart('(').TrimEnd(')').Replace(")(", "#");
        string[] array = str.Split('#');
        for (int i = 0; i < array.Length; i++)
        {
            string[] a1 = array[i].Split('|');
            string[] a2 = a1[0].Split('-');
            if (random >= a2[0].ToInt32() && random <= a2[1].ToInt32())
            {
                m.Id = idValue[0].ToInt32();
                m.Num = a1[1].ToInt32();
                list.Add(m);
                break;
            }
        }
    }
}
