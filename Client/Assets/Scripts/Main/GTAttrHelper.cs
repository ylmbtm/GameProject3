using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using Protocol;

public class GTAttrHelper
{
    static Dictionary<EAttr, int> Default { get; set; }

    static GTAttrHelper()
    {
        Default = new Dictionary<EAttr, int>();
        string[] values = Enum.GetNames(typeof(EAttr));
        for (int i = 0; i < values.Length; i++)
        {
            EAttr e = (EAttr)Enum.Parse(typeof(EAttr), values[i]);
            Default[e] = 0;
        }
    }

    static Dictionary<EAttr, int> New()
    {
        Dictionary<EAttr, int> dict = new Dictionary<EAttr, int>();
        Dictionary<EAttr, int>.Enumerator em = Default.GetEnumerator();
        while(em.MoveNext())
        {
            dict[em.Current.Key] = em.Current.Value;
        }
        em.Dispose();
        return dict;
    }

    static Dictionary<EAttr, int> Calc(Dictionary<EAttr, int> a, Dictionary<EAttr, int> b)
    {
        foreach (var current in b)
        {
            EAttr key = current.Key;
            if (a.ContainsKey(key))
            {
                a[key] = a[key] + current.Value;
            }
        }
        return a;
    }

    public static int GetFightValue(Dictionary<EAttr, int> attrValues)
    {
        int value = 0;
        Dictionary<EAttr, int>.Enumerator em = attrValues.GetEnumerator();
        while (em.MoveNext())
        {
            DProperty db = ReadCfgProperty.GetDataById(em.Current.Key);
            value += db.Factor * em.Current.Value;
        }
        em.Dispose();
        return value;
    }

    public static int GetFightValue(XEquip equip)
    {
        Dictionary<EAttr, int> attrValues = GetPropertys(equip);
        int value = GetFightValue(attrValues);
        return value;
    }

    public static int GetFightValue(XGem gem)
    {
        Dictionary<EAttr, int> attrValues = GetPropertys(gem);
        int value = GetFightValue(attrValues);
        return value;
    }

    public static int GetFightValue(XCharacter player)
    {
        Dictionary<EAttr, int> attrValues = GetPropertys(player);
        int value = GetFightValue(attrValues);
        return value;
    }

    public static int GetFightValue(XPartner role)
    {
        Dictionary<EAttr, int> attrValues = GetPropertys(role);
        int value = GetFightValue(attrValues);
        return value;
    }

    public static Dictionary<EAttr, int> GetEquipBasePropertys(int equipID)
    {
        DEquip equipDB = ReadCfgEquip.GetDataById(equipID);
        Dictionary<EAttr, int> attrValues = New();
        Dictionary<EAttr, int>.Enumerator em = equipDB.Propertys.GetEnumerator();
        while (em.MoveNext())
        {
            attrValues[em.Current.Key] = em.Current.Value;
        }
        em.Dispose();
        CalcMaxAttrValue(ref attrValues);
        return attrValues;
    }

    public static Dictionary<EAttr, int> GetGemBasePropertys(int gemID)
    {
        DGem db = ReadCfgGem.GetDataById(gemID);
        Dictionary<EAttr, int> attrValues = New();
        if (db.Propertys.Count > 0)
        {
            attrValues.Add(db.Propertys[0].Property, db.Propertys[0].Value);
        }
        CalcMaxAttrValue(ref attrValues);
        return attrValues;
    }

    public static Dictionary<EAttr, int> GetPropertys(XCharacter player)
    {
        if (player == null)
        {
            return New();
        }
        Dictionary<EAttr, int> attrValues = New();
        CalcRoleLevel(attrValues, player);
        CalcAllEquipPropertys(attrValues);
        CalcAllGemPropertys(attrValues);
        Calc(attrValues, ReadCfgActor.GetDataById(player.Id).Attrs);
        CalcMaxAttrValue(ref attrValues);
        return attrValues;
    }

    public static Dictionary<EAttr, int> GetPropertys(XEquip equip)
    {
        Dictionary<EAttr, int> attrValues = GetEquipBasePropertys(equip.Id);
        if (equip != null)
        {
            CalCfgStrengthValuethLevel(attrValues, equip);
            CalcEquipAdvanceLevel(attrValues, equip);
            CalcEquipStarLevel(attrValues, equip);
        }
        CalcMaxAttrValue(ref attrValues);
        return attrValues;
    }

    public static Dictionary<EAttr, int> GetPropertys(XGem gem)
    {
        Dictionary<EAttr, int> attrValues = New();
        CalcGemStrenthLevel(attrValues, gem);
        CalcMaxAttrValue(ref attrValues);
        return attrValues;
    }

    public static Dictionary<EAttr, int> GetPropertys(XPartner partner)
    {
        Dictionary<EAttr, int> attrValues = New();
        CalcPartnerLevel(attrValues, partner);
        CalcPartnerAdvance(attrValues, partner);
        Calc(attrValues, ReadCfgActor.GetDataById(partner.Id).Attrs);
        CalcMaxAttrValue(ref attrValues);
        return attrValues;
    }

    public static Dictionary<EAttr, int> GetAllGemPropertysByIndex(int index)
    {
        if (!DataDBSDressEquip.ContainsKey(index))
        {
            return New();
        }
        Dictionary<EAttr, int> attrValues = New();
        for (int i = 1; i <= 5; i++)
        {
            int gemPos = MLGem.Instance.GetDressGemPosByIndexAndTargetPos(index, i);
            XItem item = DataDBSDressGem.GetDataById(gemPos);
            if (item == null)
            {
                continue;
            }
            XGem gem = DataDBSGem.GetDataById(item.Instance);
            if (gem != null)
            {
                continue;
            }
            CalcGemStrenthLevel(attrValues, gem);
        }
        CalcMaxAttrValue(ref attrValues);
        return attrValues;
    }

    public static List<int>              GetPropertyValues(XCharacter player)
    {
        List<int> list = new List<int>();
        Dictionary<EAttr, int> attrValues = GetPropertys(player);
        string[] values = Enum.GetNames(typeof(EAttr));
        for (int i = 0; i < values.Length; i++)
        {
            list.Add(attrValues[(EAttr)i]);
        }
        return list;
    }

    static void CalcPartnerLevel(Dictionary<EAttr, int> dict, XPartner partner)
    {
        if(partner==null)
        {
            return;
        }
        DActor db = ReadCfgActor.GetDataById(partner.Id);
        DPartner partnerDB = ReadCfgPartner.GetDataById(partner.Id);
        if (db == null || partner.Level < 1)
        {
            return;
        }
    }

    static void CalcPartnerAdvance(Dictionary<EAttr, int> dict, XPartner partner)
    {

    }

    static void CalcRoleLevel(Dictionary<EAttr, int> dict, XCharacter player)
    {
        Dictionary<int, DRoleLevel> heroLevelDict = ReadCfgRoleLevel.Dict;
        Dictionary<EAttr, int>.Enumerator em = heroLevelDict[player.Level].Attrs.GetEnumerator();
        while (em.MoveNext())
        {
            dict[em.Current.Key] += em.Current.Value;
        }
        em.Dispose();
    }

    static void CalcEquipAdvanceLevel(Dictionary<EAttr, int> dict, XEquip equip)
    {
        int lv = equip.AdvanceLevel;
        DEquipAdvance db = ReadCfgEquipAdvance.GetDataById(equip.Id);
        for (int i = 0; i < db.Propertys.Count; i++)
        {
            if (i < lv)
            {
                KeyValuePair<EAttr, int> ep = db.Propertys[i];
                dict[ep.Key] += ep.Value;
            }
            else
            {
                break;
            }
        }
    }

    static void CalCfgStrengthValuethLevel(Dictionary<EAttr, int> dict, XEquip equip)
    {
        int lv = equip.StrengthenLevel;
        DEquip equipDB = ReadCfgEquip.GetDataById(equip.Id);
        dict[EAttr.HP] += lv * equipDB.StrengthGrow1;
        dict[EAttr.AP] += lv * equipDB.StrengthGrow2;
        dict[EAttr.DF] += lv * equipDB.StrengthGrow3;
        DEquipStreng strengthDB = ReadCfgEquipStreng.GetDataById(equip.Id);
        for (int i = 0; i < strengthDB.Propertys.Count; i++)
        {
            DStrengthValue es = strengthDB.Propertys[i];
            if (lv >= es.UnlockLevel)
            {
                dict[es.Property] += es.Value;
            }
            else
            {
                break;
            }
        }
    }

    static void CalcEquipStarLevel(Dictionary<EAttr, int> dict, XEquip equip)
    {
        int lv = equip.StarLevel;
        DEquip equipDB = ReadCfgEquip.GetDataById(equip.Id);
        DEquipStar starDB = ReadCfgEquipStar.GetDataById(equipDB.Quality * 1000 + lv);
        float percent = 1;
        if(starDB!=null)
        {
            percent = starDB.Percent / 100f + 1;
        } 
        dict[EAttr.HP] = (int)(dict[EAttr.HP] * percent);
        dict[EAttr.AP] = (int)(dict[EAttr.AP] * percent);
        dict[EAttr.DF] = (int)(dict[EAttr.DF] * percent);
    }

    static void CalcGemStrenthLevel(Dictionary<EAttr, int> dict, XGem gem)
    {
        DGem gemDB = ReadCfgGem.GetDataById(gem.Id);
        DGemLevel db = ReadCfgGemLevel.GetDataById(gemDB.Quality * 1000 + gem.StrengthenLevel);
        for (int i = 0; i < gemDB.Propertys.Count; i++)
        {
            DStrengthValue gp = gemDB.Propertys[i];
            if (gem.StrengthenLevel >= gp.UnlockLevel)
            {
                dict[gp.Property] += gp.Value;
                int ratio = gem.StrengthenLevel > 0 ? db.PropertyRatio : 100;
                dict[gp.Property] = (int)(dict[gp.Property] * ratio / 100f);
            }
            else
            {
                break;
            }
        }
    }

    static void CalcAllGemPropertys(Dictionary<EAttr, int> dict)
    {
        Dictionary<int, XItem>.Enumerator em = DataDBSDressGem.Dict.GetEnumerator();
        while (em.MoveNext())
        {
            int dressPos = em.Current.Key;
            int index = MLGem.Instance.GetIndexByDressGemPos(dressPos);
            if (!DataDBSDressEquip.ContainsKey(index))
            {
                continue;
            }
            int instanceID = em.Current.Value.Instance;
            if (!DataDBSGem.ContainsKey(instanceID))
            {
                continue;
            }
            XGem gem = DataDBSGem.GetDataById(instanceID);
            Dictionary<EAttr, int>.Enumerator em2 = GetPropertys(gem).GetEnumerator();
            while (em2.MoveNext())
            {
                dict[em2.Current.Key] += em2.Current.Value;
            }
            em2.Dispose();
        }
        em.Dispose();
    }

    static void CalcAllEquipPropertys(Dictionary<EAttr, int> dict)
    {
        Dictionary<int, XItem> dressEquips = DataDBSDressEquip.Dict;
        Dictionary<int, XItem>.Enumerator em1 = dressEquips.GetEnumerator();
        while (em1.MoveNext())
        {
            if (em1.Current.Value.Instance == 0)
            {
                continue;
            }
            int instanceID = em1.Current.Value.Instance;
            if (!DataDBSEquip.ContainsKey(instanceID))
            {
                continue;
            }
            XEquip equip = DataDBSEquip.GetDataById(instanceID);
            Dictionary<EAttr, int>.Enumerator em2 = GetPropertys(equip).GetEnumerator();
            while (em2.MoveNext())
            {
                dict[em2.Current.Key] += em2.Current.Value;
            }
            em2.Dispose();
        }
        em1.Dispose();
    }

    static void CalcMaxAttrValue(ref Dictionary<EAttr, int> dict)
    {
        dict[EAttr.MAXHP] = dict[EAttr.HP];
        dict[EAttr.MAXMP] = dict[EAttr.MP];
        dict[EAttr.MAXSP] = dict[EAttr.SP];
    }
}