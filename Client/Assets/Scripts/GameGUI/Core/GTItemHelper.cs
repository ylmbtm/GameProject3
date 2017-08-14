using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using Protocol;

public class GTItemHelper
{
    public static void ShowStarNum(List<UISprite> stars, int starNum)
    {
        if (stars == null) return;
        for (int i = 0; i < stars.Count; i++)
        {
            UISprite sprite = stars[i];
            sprite.gameObject.SetActive(i < starNum);
        }
    }

    public static void ShowStarNum(UISprite [] stars,int starNum)
    {
        if (stars == null) return;
        for (int i = 0; i < stars.Length; i++)
        {
            UISprite sprite = stars[i];
            sprite.gameObject.SetActive(true);
            sprite.spriteName = i < starNum ? "S2" : "S1";
        }
    }

    public static void ShowQualityText(UILabel lab,string text,int quality)
    {
        if(string.IsNullOrEmpty(text)||lab==null)
        {
            return;
        }
        string encode = ReadCfgQuality.GetDataById(quality).Encode;
        lab.text = GTTools.Format("[{0}]{1}[-]", encode, text);
    }

    public static void ShowItemQuality(UISprite sprite, int itemID)
    {
        if (sprite == null) return;
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        DQuality db = ReadCfgQuality.GetDataById(itemDB.Quality);
        sprite.gameObject.SetActive(true);
        sprite.spriteName = db.Icon;
    }

    public static void ShowItemQuality(UILabel la, int quality)
    {
        if (la == null) return;
        DQuality db = ReadCfgQuality.GetDataById(quality);
        if (db == null) return;
        la.text = GTTools.Format("[{0}]{1}[-]", db.Encode, db.Desc);
    }

    public static void ShowItemChip(GameObject go, int itemID)
    {
        if (go == null) return;
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        if(itemDB==null)
        {
            return;
        }
        go.SetActive(itemDB.ItemType == EItemType.CHIP);
    }

    public static void ShowActorQuality(UISprite sprite, int id)
    {
        if (sprite == null) return;
        DActor db = ReadCfgActor.GetDataById(id);
        sprite.spriteName = db == null ? "P0" : "P" + db.Quality;
    }

    public static void ShowItemNum(UILabel lab, int num)
    {
        if (lab == null) return;
        lab.gameObject.SetActive(true);
        lab.text = num > 1 ? num.ToString() : string.Empty;
    }

    public static void ShowProgressText(UILabel lab, int now, int max)
    {
        if (lab == null) return;
        lab.gameObject.SetActive(true);
        lab.text = GTTools.Format("{0}/{1}", now, max);
    }

    public static void ShowPriceText(UILabel lab, int now, int max)
    {
        if (lab == null) return;
        lab.gameObject.SetActive(true);
        string color = now >= max ? "00ff00" : "ff0000";
        lab.text = GTTools.Format("[{0}]{1}[-]/{2}", color, now, max);
    }

    public static void ShowProgressSlider(UISlider sdr, int now, int max)
    {
        if (sdr == null) return;
        sdr.value = now / (max * 1f);
    }

    public static void ShowItemTexture(UITexture tex, int itemID)
    {
        if (tex == null) return;
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        Texture2D t2d = GTResourceManager.Instance.Load<Texture2D>(string.Format("Textures/Item/{0}", itemDB.Icon));
        if (t2d == null)
        {
            t2d = new Texture2D(64, 64, TextureFormat.ARGB32, false);
            t2d.wrapMode = TextureWrapMode.Clamp;
            t2d.name = "None";
        }
        tex.mainTexture = t2d;
        tex.gameObject.SetActive(true);
    }

    public static void ShowTexture(UITexture tex,string path)
    {
        if (tex == null) return;
        Texture2D t2d = GTResourceManager.Instance.Load<Texture2D>(path);
        tex.mainTexture = t2d;
        tex.gameObject.SetActive(true);
    }

    public static void ShowItemName(UILabel lab, int itemID)
    {
        if (lab == null) return;
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        if (itemDB == null) return;
        string encode = ReadCfgQuality.GetDataById(itemDB.Quality).Encode;
        lab.text = GTTools.Format("[{0}]{1}[-]", encode, itemDB.Name);
    }

    public static void ShowItemName(UILabel lab, int quality,string name)
    {
        string encode = ReadCfgQuality.GetDataById(quality).Encode;
        lab.text = GTTools.Format("[{0}]{1}[-]", encode, name);
    }

    public static void ShowPropertyText(UILabel label, Dictionary<EAttr, int> dict, bool showAll = true)
    {
        if (dict == null) return;
        if (label == null) return;
        Dictionary<EAttr, int>.Enumerator em = dict.GetEnumerator();
        label.text = string.Empty;
        while (em.MoveNext())
        {
            EAttr e = em.Current.Key;
            int value = em.Current.Value;
            if (em.Current.Value <= 0 || ReadCfgProperty.ContainsKey(e) == false)
            {
                continue;
            }
            DProperty db = ReadCfgProperty.GetDataById(e);
            string s = GTTools.Format(db.Desc, value);
            switch (e)
            {
                case EAttr.AP:
                case EAttr.DF:
                case EAttr.HP:
                    label.Append(s);
                    break;
                case EAttr.CRIT:
                case EAttr.CRITDAMAGE:
                case EAttr.DODGE:
                case EAttr.HIT:
                case EAttr.SUCK:
                case EAttr.MP:
                    if (showAll)
                    {
                        string str = GTTools.Format("[00ff00]装备：{0}[-]", s);
                        label.Append(str);
                    }
                    break;
            }
        }
    }

    public static void ShowEquipStrengthText(UILabel label, int equipID, int strengthLevel)
    {
        if (label == null) return;
        DEquipStreng strengthDB = ReadCfgEquipStreng.GetDataById(equipID);
        label.text = string.Empty;
        for (int i = 0; i < strengthDB.Propertys.Count; i++)
        {
            DStrengthValue data = strengthDB.Propertys[i];
            if (data.Value <= 0 || data.Property == 0)
            {
                continue;
            }
            DProperty propertyDB = ReadCfgProperty.GetDataById(data.Property);
            string desc = GTTools.Format(propertyDB.Desc, data.Value);
            string v = string.Empty;
            if (data.UnlockLevel <= strengthLevel)
            {
                v = GTTools.Format("[00ff00]{0}[-]", desc);
            }
            else
            {
                v = GTTools.Format("[777777]{0} (强化至{1}级生效)[-]", desc, data.UnlockLevel);

            }
            label.Append(v);
        }
    }

    public static void ShowEquipAdvanceText(UILabel label, int equipID, int advanceLevel)
    {
        if (label == null) return;
        DEquipAdvance advanceDB = ReadCfgEquipAdvance.GetDataById(equipID);
        label.text = string.Empty;
        for (int i = 0; i < advanceDB.Propertys.Count; i++)
        {
            KeyValuePair<EAttr, int> data = advanceDB.Propertys[i];
            if (data.Value <= 0 || data.Key == 0)
            {
                continue;
            }
            DProperty propertyDB = ReadCfgProperty.GetDataById(data.Key);
            string desc = GTTools.Format(propertyDB.Desc, data.Value);
            string v = string.Empty;
            if (advanceLevel >= (i + 1))
            {

                v = GTTools.Format("[00ff00]{0}[-]", desc);
            }
            else
            {
                v = GTTools.Format("[777777]{0} (进阶至{1}级生效)[-]", desc, i + 1);
            }
            label.Append(v);
        }
    }

    public static void ShowEquipStarText(UILabel label, int equipID, int starLevel, bool showAll = true)
    {
        if (label == null) return;
        label.text = string.Empty;
        DEquip equipDB = ReadCfgEquip.GetDataById(equipID);

        if (showAll)
        {
            for (int i = 0; i < 5; i++)
            {
                int lv = i + 1;
                DEquipStar starDB = ReadCfgEquipStar.GetDataById(equipDB.Quality * 1000 + lv);
                float percent = starDB == null ? 0 : starDB.Percent;
                string desc = GTTools.Format("生命、攻击和防御+{0}%", percent);
                string v = string.Empty;
                if (starLevel >= lv)
                {
                    v = GTTools.Format("[00ff00]{0}[-]", desc);
                }
                else
                {
                    v = GTTools.Format("[777777]{0} (升星至{1}级生效)[-]", desc, lv);
                }
                label.Append(v);
            }
        }
        else
        {
            DEquipStar starDB = ReadCfgEquipStar.GetDataById(equipDB.Quality * 1000 + starLevel);
            string format = "{0}提高 [00ff00]{1}[-] %";
            float percent = starDB == null ? 0 : starDB.Percent;
            label.Append(GTTools.Format(format, "生命", percent));
            label.Append(GTTools.Format(format, "攻击", percent));
            label.Append(GTTools.Format(format, "防御", percent));
        }
    }

    public static void ShowGemPropertyText(UILabel label, int gemID, int level, bool showNext = false)
    {
        if (label.text == null) return;
        label.text = string.Empty;
        DGem gemDB = ReadCfgGem.GetDataById(gemID);
        DGemLevel db1 = ReadCfgGemLevel.GetDataById(gemDB.Quality * 1000 + level);
        DGemLevel db2 = ReadCfgGemLevel.GetDataById(gemDB.Quality * 1000 + level + 1);
        for (int i = 0; i < gemDB.Propertys.Count; i++)
        {
            DStrengthValue gp = gemDB.Propertys[i];
            if (!ReadCfgProperty.ContainsKey(gp.Property))
            {
                continue;
            }
            DProperty propertyDB = ReadCfgProperty.GetDataById(gp.Property);
            int ratio = level > 0 ? db1.PropertyRatio : 100;
            string s = GTTools.Format(propertyDB.Desc, (int)(gp.Value * ratio / 100f));
            int add = (int)(gp.Value * (db2.PropertyRatio - ratio) / 100f);
            string str = string.Empty;
            if (level >= gp.UnlockLevel)
            {
                if (showNext)
                {
                    str = GTTools.Format("[00ff00]{0}(+{1})[-]", s, add);
                }
                else
                {
                    str = GTTools.Format("[00ff00]{0}[-]", s);
                }
            }
            else
            {
                str = GTTools.Format("[777777]{0} (宝石强化至{1}级生效)[-]", s, gp.UnlockLevel);
            }
            label.Append(str);
        }
        label.text = label.text.TrimEnd('\n');
    }

    public static bool CheckBagFull(int addBagNum, EBagType bagType, bool showTip = true)
    {
        Dictionary<int, XItem> itemData = GTDataManager.Instance.GetBagDataByBagType(bagType);
        if (addBagNum + itemData.Count >= GTDataManager.Instance.GetBagNum())
        {
            if (showTip)
            {
                GTItemHelper.ShowTip("背包空间不足");
            }
            return true;
        }
        return false;
    }

    public static bool CheckBagFull(int addBagNum, bool showTip = true)
    {
        for (int i = 0; i < Enum.GetValues(typeof(EBagType)).Length; i++)
        {
            if (i == 0) continue;
            EBagType bagType = (EBagType)i;
            if (CheckBagFull(addBagNum, bagType , showTip))
            {
                return true;
            }
        }
        return false;
    }

    public static bool CheckItemEnongh(int itemID, int costNum)
    {
        if (itemID == 0)
        {
            return true;
        }
        if (GTDataManager.Instance.GetItemCountById(itemID) < costNum)
        {
            DItem itemDB = ReadCfgItem.GetDataById(itemID);
            GTItemHelper.ShowTip("物品不足：" + itemDB.Name);
            return false;
        }
        return true;
    }

    public static void ShowImageBlack(UIBasicSprite sprite, bool black)
    {
        if (sprite == null)
        {
            return;
        }
        sprite.color = black ? new Color(0, 1, 1) : Color.white;
    }

    public static void ShowItemDialogById(int itemID)
    {
        DItem db = ReadCfgItem.GetDataById(itemID);
        switch (db.ItemType)
        {
            case EItemType.EQUIP:
                break;
            case EItemType.DRUG:
            case EItemType.MAT:
            case EItemType.ACTION:
            case EItemType.MONEY:
            case EItemType.KEY:
                GTWindowManager.Instance.OpenWindow(EWindowID.UIItemInfo);
                UIItemInfo itemInfo = (UIItemInfo)GTWindowManager.Instance.GetWindow(EWindowID.UIItemInfo);
                itemInfo.ShowViewById(itemID);
                break;
            case EItemType.GEM:
                break;
            case EItemType.CHIP:
                break;
            case EItemType.FASHION:
                break;
            case EItemType.EXP:
                break;
            case EItemType.BOX:
                break;
        }
    }

    public static void ShowItemDialogByBagType(EBagType bagType, int pos)
    {
        Dictionary<int, XItem> dict = GTDataManager.Instance.GetBagDataByBagType(bagType);
        if (!dict.ContainsKey(pos)) return;
        XItem item = dict[pos];
        DItem db = ReadCfgItem.GetDataById(item.Id);
        switch (db.ItemType)
        {
            case EItemType.EQUIP:
                GTWindowManager.Instance.OpenWindow(EWindowID.UIEquipInfo);
                UIEquipInfo w1 = (UIEquipInfo)GTWindowManager.Instance.GetWindow(EWindowID.UIEquipInfo);
                w1.ShowViewByPos(EPosType.BagItem, pos);
                break;
            case EItemType.MAT:
                GTWindowManager.Instance.OpenWindow(EWindowID.UIItemInfo);
                UIItemInfo w2 = (UIItemInfo)GTWindowManager.Instance.GetWindow(EWindowID.UIItemInfo);
                w2.ShowViewByPos(pos, false);
                break;
            case EItemType.DRUG:
            case EItemType.BOX:
            case EItemType.KEY:
                GTWindowManager.Instance.OpenWindow(EWindowID.UIItemInfo);
                UIItemInfo w3 = (UIItemInfo)GTWindowManager.Instance.GetWindow(EWindowID.UIItemInfo);
                w3.ShowViewByPos(pos, true);
                break;
            case EItemType.EXP:
                break;
            case EItemType.CHIP:
                GTWindowManager.Instance.OpenWindow(EWindowID.UIChipInfo);
                UIChipInfo w4 = (UIChipInfo)GTWindowManager.Instance.GetWindow(EWindowID.UIChipInfo);
                w4.ShowViewByPos(pos);
                break;
            case EItemType.GEM:
                GTWindowManager.Instance.OpenWindow(EWindowID.UIGemInfo);
                UIGemInfo w5 = (UIGemInfo)GTWindowManager.Instance.GetWindow(EWindowID.UIGemInfo);
                w5.ShowViewByPos(EPosType.BagGem, pos);
                break;
        }
    }

    public static void ShowItemDialogByPosType(EPosType posType, int pos)
    {
        Dictionary<int, XItem> dict = GTDataManager.Instance.GetItemDataByPosType(posType);
        if (!dict.ContainsKey(pos)) return;
        switch (posType)
        {
            case EPosType.RoleEquip:
                GTWindowManager.Instance.OpenWindow(EWindowID.UIEquipInfo);
                UIEquipInfo window1 = (UIEquipInfo)GTWindowManager.Instance.GetWindow(EWindowID.UIEquipInfo);
                window1.ShowViewByPos(EPosType.RoleEquip, pos);
                break;
            case EPosType.RoleGem:
                GTWindowManager.Instance.OpenWindow(EWindowID.UIGemInfo);
                UIGemInfo window2 = (UIGemInfo)GTWindowManager.Instance.GetWindow(EWindowID.UIGemInfo);
                window2.ShowViewByPos(EPosType.RoleGem, pos);
                break;
            case EPosType.RoleFashion:
                break;
            case EPosType.RoleRune:
                break;
            case EPosType.BagItem:
                ShowItemDialogByBagType(EBagType.ITEM, pos);
                break;
            case EPosType.BagGem:
                GTWindowManager.Instance.OpenWindow(EWindowID.UIGemInfo);
                UIGemInfo window6 = (UIGemInfo)GTWindowManager.Instance.GetWindow(EWindowID.UIGemInfo);
                window6.ShowViewByPos(EPosType.BagGem, pos);
                break;
            case EPosType.BagChip:
                GTWindowManager.Instance.OpenWindow(EWindowID.UIChipInfo);
                UIChipInfo window7 = (UIChipInfo)GTWindowManager.Instance.GetWindow(EWindowID.UIChipInfo);
                window7.ShowViewByPos(pos);
                break;
            case EPosType.BagFashion:

                break;
            case EPosType.BagRune:
                break;
            default:
                break;
        }
    }

    public static void ShowItemUseDialogByPos(int pos)
    {
        if (!DataDBSBagItem.ContainsKey(pos)) return;
        GTWindowManager.Instance.OpenWindow(EWindowID.UIItemUse);
        UIItemUse window = (UIItemUse)GTWindowManager.Instance.GetWindow(EWindowID.UIItemUse);
        window.ShowView(pos);
    }

    public static void ShowEquipWindowByPos(EPosType posType, int pos)
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UIEquip);
        UIEquip window = (UIEquip)GTWindowManager.Instance.GetWindow(EWindowID.UIEquip);
        window.ShowEquipWindowByPos(posType, pos);
    }

    public static void ShowGemWindowByPos(EPosType posType, int pos)
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UIGem);
        UIGem window = (UIGem)GTWindowManager.Instance.GetWindow(EWindowID.UIGem);
        window.ShowWindowByPos(posType, pos);
    }

    public static void ShowTip(string key)
    {
        string value = GetText(key);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIMessageTip);
        UIMessageTip dialog = (UIMessageTip)GTWindowManager.Instance.GetWindow(EWindowID.UIMessageTip);
        dialog.ShowWarning(value);
    }

    public static void ShowExpTip(int exp)
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UIMessageTip);
        UIMessageTip dialog = (UIMessageTip)GTWindowManager.Instance.GetWindow(EWindowID.UIMessageTip);
        string format = GetText("经验+{0}");
        dialog.ShowExp(GTTools.Format(format, exp));
    }

    public static void ShowAwardTip(List<GTItem> list)
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UIAwardTip);
        UIAwardTip dialog = (UIAwardTip)GTWindowManager.Instance.GetWindow(EWindowID.UIAwardTip);
        dialog.ShowView(list);
    }

    public static void ShowAwardTip(GTItem data)
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UIAwardTip);
        UIAwardTip dialog = (UIAwardTip)GTWindowManager.Instance.GetWindow(EWindowID.UIAwardTip);
        dialog.ShowView(new List<GTItem> { data });
    }

    public static void ShowMessageBox(string title, string content, Callback ok, Callback cancle)
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UIMessageBox);
        UIMessageBox dialog = (UIMessageBox)GTWindowManager.Instance.GetWindow(EWindowID.UIMessageBox);
        dialog.ShowView(title, content, ok, cancle);
    }

    public static bool ShowNetworkError(uint errorCode)
    {
        if (errorCode != 0)
        {
            GTItemHelper.ShowTip(string.Format("网络错误：{0}", errorCode));
            return false;
        }
        else
        {
            return true;
        }
    }

    public static string GetText(string key)
    {
        DLocalString cfg = ReadCfgLocalString.GetDataById(key);
        return cfg == null ? key : cfg.Value;
    }
}
