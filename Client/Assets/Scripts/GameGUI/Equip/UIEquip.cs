using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIEquip : GTWindow
{
    private GameObject        menuBtn1;
    private GameObject        menuBtn2;
    private GameObject        menuBtn3;
    private GameObject        btnClose;
    private UILabel           currFuncName;

    private UCEquipStrengthen equipStrengthen        = new UCEquipStrengthen();
    private UCEquipAdvance    equipAdvance           = new UCEquipAdvance();
    private UCEquipStar       equipStar              = new UCEquipStar();

    private EquipFunc         curEquipFunc           = EquipFunc.STRENGTHEN;
    private List<XItem>       curStrengthenCostItems = new List<XItem>();
    private int               mPos;
    private EPosType          mPosType;

    enum EquipFunc
    {
        STRENGTHEN,
        ADVANCE,
        EQUIPSTAR,
    }

    public UIEquip()
    {
        MaskType = EWindowMaskType.Black;
        Type = EWindowType.Window;
        Resident = false;
        Path = "Equip/UIEquip";
        ShowMode = EWindowShowMode.HideOther;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");

        menuBtn1 = pivot.Find("MenuBtn/MenuBtn_1").gameObject;
        menuBtn2 = pivot.Find("MenuBtn/MenuBtn_2").gameObject;
        menuBtn3 = pivot.Find("MenuBtn/MenuBtn_3").gameObject;
        btnClose = pivot.Find("BtnClose").gameObject;
        currFuncName = pivot.Find("Title").GetComponent<UILabel>();

        Transform strengthen = pivot.Find("View1");
        equipStrengthen.panel = strengthen.gameObject;
        equipStrengthen.equipTexture = strengthen.Find("Equip/Texture").GetComponent<UITexture>();
        equipStrengthen.equipQuality = strengthen.Find("Equip/Quality").GetComponent<UISprite>();
        equipStrengthen.costMoneyTexture = strengthen.Find("Cost/Texture").GetComponent<UITexture>();
        equipStrengthen.costMoneyNum = strengthen.Find("Cost/Num").GetComponent<UILabel>();
        equipStrengthen.btnAutoToAddMat = strengthen.Find("Btn_AutoToAddMat").gameObject;
        equipStrengthen.btnStrengthen = strengthen.Find("Btn_Strengthen").gameObject;
        equipStrengthen.btnStrengthenTenTimes = strengthen.Find("Btn_StrengthenTenTimes").gameObject;
        equipStrengthen.equipName = strengthen.Find("EquipName").GetComponent<UILabel>();

        equipStrengthen.equipStrengthExpBar = strengthen.Find("Content0").GetComponent<UISlider>();
        equipStrengthen.equipStrengthLevel = strengthen.Find("Content0/Level").GetComponent<UILabel>();
        equipStrengthen.equipStrengthExp = strengthen.Find("Content0/Num").GetComponent<UILabel>();
        equipStrengthen.propertyText = strengthen.Find("Content1/Text").GetComponent<UILabel>();
        equipStrengthen.strengthText = strengthen.Find("Content2/Text").GetComponent<UILabel>();

        Transform itemsTrans = strengthen.Find("Items");
        for (int i = 1; i <= 6; i++)
        {
            UCEquipItemStrengthen item = new UCEquipItemStrengthen();
            item.itemBtn = itemsTrans.Find("Item_" + i).gameObject;
            item.itemTexture = item.itemBtn.transform.Find("Texture").GetComponent<UITexture>();
            item.itemQuality = item.itemBtn.transform.Find("Quality").GetComponent<UISprite>();
            item.itemNum = item.itemBtn.transform.Find("Num").GetComponent<UILabel>();
            item.itemPlus = item.itemBtn.transform.Find("Plus").gameObject;
            equipStrengthen.strengthenItems.Add(item);
        }


        Transform advance = pivot.Find("View2");
        equipAdvance.panel = advance.gameObject;
        equipAdvance.nowEquipTexture = advance.Find("Now/Equip/Texture").GetComponent<UITexture>();
        equipAdvance.nowEquipQuality = advance.Find("Now/Equip/Quality").GetComponent<UISprite>();
        equipAdvance.nowEquipName = advance.Find("Now/EquipName").GetComponent<UILabel>();
        equipAdvance.nowEquipFightValue = advance.Find("Now/FightValue/Num").GetComponent<UILabel>();
        equipAdvance.nowEquipPropertys = advance.Find("Now/Propertys").GetComponent<UILabel>();
        equipAdvance.nexEquipTexture = advance.Find("Next/Equip/Texture").GetComponent<UITexture>();
        equipAdvance.nexEquipQuality = advance.Find("Next/Equip/Quality").GetComponent<UISprite>();
        equipAdvance.nexEquipName = advance.Find("Next/EquipName").GetComponent<UILabel>();
        equipAdvance.nexEquipFightValue = advance.Find("Next/FightValue/Num").GetComponent<UILabel>();
        equipAdvance.nexEquipPropertys = advance.Find("Next/Propertys").GetComponent<UILabel>();
        equipAdvance.btnAdvance = advance.Find("Btn_Advance").gameObject;

        equipAdvance.costEquipBtn = advance.Find("Cost/Equip").gameObject;
        equipAdvance.costEquipNum = advance.Find("Cost/Equip/Num").GetComponent<UILabel>();
        equipAdvance.costEquipTexture = advance.Find("Cost/Equip/Texture").GetComponent<UITexture>();
        equipAdvance.costEquipQuality = advance.Find("Cost/Equip/Quality").GetComponent<UISprite>();

        equipAdvance.costItemBtn = advance.Find("Cost/Item").gameObject;
        equipAdvance.costItemNum = advance.Find("Cost/Item/Num").GetComponent<UILabel>();
        equipAdvance.costItemTexture = advance.Find("Cost/Item/Texture").GetComponent<UITexture>();
        equipAdvance.costItemQuality = advance.Find("Cost/Item/Quality").GetComponent<UISprite>();
        equipAdvance.costMoneyNum = advance.Find("Btn_Advance/Num").GetComponent<UILabel>();
        equipAdvance.costMoneyTexture=advance.Find("Btn_Advance/Texture").GetComponent<UITexture>();
        equipAdvance.advanceText = advance.Find("Content0/Text").GetComponent<UILabel>();



        Transform equipStar = pivot.Find("View3");
        this.equipStar.panel = equipStar.gameObject;
        this.equipStar.nowEquipTexture = equipStar.Find("Now/Equip/Texture").GetComponent<UITexture>();
        this.equipStar.nowEquipQuality = equipStar.Find("Now/Equip/Quality").GetComponent<UISprite>();
        this.equipStar.nowEquipName = equipStar.Find("Now/EquipName").GetComponent<UILabel>();
        this.equipStar.nowEquipPropertys = equipStar.Find("Now/Propertys").GetComponent<UILabel>();
        this.equipStar.nexEquipTexture = equipStar.Find("Next/Equip/Texture").GetComponent<UITexture>();
        this.equipStar.nexEquipQuality = equipStar.Find("Next/Equip/Quality").GetComponent<UISprite>();
        this.equipStar.nexEquipName = equipStar.Find("Next/EquipName").GetComponent<UILabel>();
        this.equipStar.nexEquipPropertys = equipStar.Find("Next/Propertys").GetComponent<UILabel>();

        equipStar.Find("Now/Stars").GetComponentsInChildren(this.equipStar.nowEquipStars);
        equipStar.Find("Next/Stars").GetComponentsInChildren(this.equipStar.nexEquipStars);

        this.equipStar.btnUpStar = equipStar.Find("Btn_UpStar").gameObject;
        this.equipStar.costMoneyNum = equipStar.Find("Btn_UpStar/Num").GetComponent<UILabel>();
        this.equipStar.costMoneyTexture = equipStar.Find("Btn_UpStar/Texture").GetComponent<UITexture>();

        this.equipStar.costItemBtn = equipStar.Find("Cost/Item").gameObject;
        this.equipStar.costItemTexture = equipStar.Find("Cost/Item/Texture").GetComponent<UITexture>();
        this.equipStar.costItemQuality = equipStar.Find("Cost/Item/Quality").GetComponent<UISprite>();
        this.equipStar.costItemNum = equipStar.Find("Cost/Item/Num").GetComponent<UILabel>();
        this.equipStar.starText = equipStar.Find("Content0/Text").GetComponent<UILabel>();

    }

    protected override void OnEnable()
    {
        InitMenuBtns();
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_BAG_STRENGTHEN_EQUIP, OnRecvStrengthenEquip);
        GTEventCenter.AddHandler(GTEventID.TYPE_BAG_ADVANCE_EQUIP,    OnRecvAdvanceEquip);
        GTEventCenter.AddHandler(GTEventID.TYPE_BAG_UPSTAR_EQUIP,     OnRecvUpStarEquip);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_BAG_STRENGTHEN_EQUIP, OnRecvStrengthenEquip);
        GTEventCenter.DelHandler(GTEventID.TYPE_BAG_ADVANCE_EQUIP,    OnRecvAdvanceEquip);
        GTEventCenter.DelHandler(GTEventID.TYPE_BAG_UPSTAR_EQUIP,     OnRecvUpStarEquip);
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(menuBtn1).onClick = OnStrengthenClick;
        UIEventListener.Get(menuBtn2).onClick = OnAdvanceClick;
        UIEventListener.Get(menuBtn3).onClick = OnEquipStarClick;
        UIEventListener.Get(equipStrengthen.btnAutoToAddMat).onClick = OnAutoToAddMatClick;
        UIEventListener.Get(equipStrengthen.btnStrengthen).onClick = OnStrengthenEquipClick;
        UIEventListener.Get(equipAdvance.btnAdvance).onClick = OnAdvanceEquipClick;
        UIEventListener.Get(equipStar.btnUpStar).onClick = OnUpStarEquipClick;
    }

    protected override void OnClose()
    {
        curEquipFunc = EquipFunc.STRENGTHEN;
        curStrengthenCostItems.Clear();
        equipStrengthen.OnClose();
        equipAdvance.OnClose();
        equipStar.OnClose();
    }

    private void InitMenuBtns()
    {
        int group = GTWindowManager.Instance.GetToggleGroupId();
        UIToggle toggle1 = menuBtn1.GetComponent<UIToggle>();
        UIToggle toggle2 = menuBtn2.GetComponent<UIToggle>();
        UIToggle toggle3 = menuBtn3.GetComponent<UIToggle>();
        toggle1.group = group;
        toggle2.group = group;
        toggle3.group = group;
        toggle1.value = true;
    }

    private void OnAutoToAddMatClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (curStrengthenCostItems.Count > 0)
        {
            return;
        }
        XEquip equip = GTDataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        List<XItem> list = MLEquip.Instance.GetItemListToOneKeyStrengthen(equip);
        if (list.Count == 0)
        {
            GTItemHelper.ShowTip("没有蓝绿装，且没有水晶");
            return;
        }
        curStrengthenCostItems = list;
        ShowStrengthenView();
    }

    private void OnMoneyPlusClick2(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnMoneyPlusClick1(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnEquipStarClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        TrySwitchFunc(EquipFunc.EQUIPSTAR);
    }

    private void OnAdvanceClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        TrySwitchFunc(EquipFunc.ADVANCE);
    }

    private void OnStrengthenClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        TrySwitchFunc(EquipFunc.STRENGTHEN);
    }

    private void OnStrengthenEquipClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        XEquip equip = GTDataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        GTNetworkSend.Instance.TryStrengthEquip(equip, curStrengthenCostItems);
    }

    private void OnAdvanceEquipClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        XEquip equip = GTDataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        GTNetworkSend.Instance.TryAdvanceEquip(equip);
    }

    private void OnUpStarEquipClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        XEquip equip = GTDataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        GTNetworkSend.Instance.TryUpStarEquip(equip);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void ShowStrengthenView()
    {
        XEquip equip = GTDataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        if (equip == null) return;
        if (MLEquip.Instance.IsFullStrengthenLevel(equip) == true)
        {
            return;
        }
        int itemID = equip.Id;   
        GTItemHelper.ShowItemTexture(equipStrengthen.equipTexture, itemID);
        GTItemHelper.ShowItemQuality(equipStrengthen.equipQuality, itemID);
        GTItemHelper.ShowItemName(equipStrengthen.equipName, itemID);

        int costEquipNum = curStrengthenCostItems.Count;
        for(int i=0;i<equipStrengthen.strengthenItems.Count;i++)
        {
      
            UCEquipItemStrengthen cell = equipStrengthen.strengthenItems[i];
            if(i<costEquipNum)
            {
                XItem mat = curStrengthenCostItems[i];
                cell.Show(true);
                int id = curStrengthenCostItems[i].Id;
                GTItemHelper.ShowItemTexture(cell.itemTexture, id);
                GTItemHelper.ShowItemQuality(cell.itemQuality, id);
                GTItemHelper.ShowItemNum(cell.itemNum, mat.Num);          
            }
            else
            {
                cell.Show(false);
            }
        }
        DEquip equipDB = ReadCfgEquip.GetDataById(itemID);
        DEquipStrengLevel levelDB = ReadCfgEquipStrengLevel.GetDataById(equipDB.Quality * 1000 + equip.StrengthenLevel + 1);
        int addExpNum = MLEquip.Instance.GetStrengthenExpByMatList(curStrengthenCostItems);
        GTItemHelper.ShowItemTexture(equipStrengthen.costMoneyTexture, DEquip.EQUIP_STRENGTHEN_MONEY_ID_1);
        equipStrengthen.costMoneyNum.text = addExpNum.ToString();
        equipStrengthen.equipStrengthLevel.text = GTTools.Format("强化等级 {0}", equip.StrengthenLevel);
        GTItemHelper.ShowProgressText(equipStrengthen.equipStrengthExp, equip.StrengthenExp, levelDB.RequireExp);
        GTItemHelper.ShowProgressSlider(equipStrengthen.equipStrengthExpBar, equip.StrengthenExp, levelDB.RequireExp);
        Dictionary<EAttr, int> propertys = GTAttrHelper.GetPropertys(equip);
        GTItemHelper.ShowEquipStrengthText(equipStrengthen.strengthText, itemID, equip.StrengthenLevel);
        GTItemHelper.ShowPropertyText(equipStrengthen.propertyText, propertys, true);
    }

    private void ShowEquipStarView()
    {
        XEquip equip = GTDataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        if (equip == null) return;
        int itemID = equip.Id;
        if (MLEquip.Instance.IsFullAdvanceLevel(equip) == true)
        {
            return;
        }
        GTItemHelper.ShowItemTexture(equipStar.nowEquipTexture, itemID);
        GTItemHelper.ShowItemQuality(equipStar.nowEquipQuality, itemID);
        GTItemHelper.ShowItemName(equipStar.nowEquipName, itemID);
        GTItemHelper.ShowItemTexture(equipStar.nexEquipTexture, itemID);
        GTItemHelper.ShowItemQuality(equipStar.nexEquipQuality, itemID);
        GTItemHelper.ShowItemName(equipStar.nexEquipName, itemID);
        GTItemHelper.ShowStarNum(equipStar.nowEquipStars, equip.StarLevel);
        GTItemHelper.ShowStarNum(equipStar.nexEquipStars, equip.StarLevel + 1);

        DEquip equipDB = ReadCfgEquip.GetDataById(equip.Id);
        DEquipStar nexStarDB = ReadCfgEquipStar.GetDataById(equipDB.Quality * 1000 + equip.StarLevel + 1);

        int hasItemNum = GTDataManager.Instance.GetItemCountById(nexStarDB.CostItemId);
        GTItemHelper.ShowItemTexture(equipStar.costItemTexture, nexStarDB.CostItemId);
        GTItemHelper.ShowItemQuality(equipStar.costItemQuality, nexStarDB.CostItemId);
        GTItemHelper.ShowPriceText(equipStar.costItemNum, hasItemNum, nexStarDB.CostItemNum);

        GTItemHelper.ShowItemTexture(equipStar.costMoneyTexture, nexStarDB.CostMoneyId);
        equipStar.costMoneyNum.text = nexStarDB.CostMoneyNum.ToString();
        GTItemHelper.ShowEquipStarText(equipStar.starText, itemID, equip.StarLevel);
        GTItemHelper.ShowEquipStarText(equipStar.nowEquipPropertys, itemID, equip.StarLevel, false);
        GTItemHelper.ShowEquipStarText(equipStar.nexEquipPropertys, itemID, equip.StarLevel + 1, false);
    }

    private void ShowAdvanceView()
    {
        XEquip equip = GTDataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        if (equip == null) return;
        int itemID = equip.Id;
        if (MLEquip.Instance.IsFullStarLevel(equip))
        {
            return;
        }
        GTItemHelper.ShowItemTexture(equipAdvance.nowEquipTexture, itemID);
        GTItemHelper.ShowItemQuality(equipAdvance.nowEquipQuality, itemID);
        GTItemHelper.ShowItemName(equipAdvance.nowEquipName, itemID);

        GTItemHelper.ShowItemTexture(equipAdvance.nexEquipTexture, itemID);
        GTItemHelper.ShowItemQuality(equipAdvance.nexEquipQuality, itemID);
        GTItemHelper.ShowItemName(equipAdvance.nexEquipName, itemID);

        Dictionary<EAttr, int> nowPropertys = GTAttrHelper.GetPropertys(equip);
        XEquip nexEquip = new XEquip();
        GTTools.Copy(nexEquip, equip);
        nexEquip.AdvanceLevel++;
        Dictionary<EAttr, int> nexPropertys = GTAttrHelper.GetPropertys(nexEquip);

        GTItemHelper.ShowPropertyText(equipAdvance.nowEquipPropertys, nowPropertys, false);
        GTItemHelper.ShowPropertyText(equipAdvance.nexEquipPropertys, nexPropertys, false);
        equipAdvance.nowEquipFightValue.text = GTAttrHelper.GetFightValue(nowPropertys).ToString();
        equipAdvance.nexEquipFightValue.text = GTAttrHelper.GetFightValue(nexPropertys).ToString();

        DEquip equipDB = ReadCfgEquip.GetDataById(itemID);
        DEquipAdvanceCost db = ReadCfgEquipAdvanceCost.GetDataById(equipDB.Quality*1000+ equip.AdvanceLevel + 1);
        int hasItemNum = GTDataManager.Instance.GetItemCountById(db.CostItemId);
        GTItemHelper.ShowItemTexture(equipAdvance.costItemTexture, db.CostItemId);
        GTItemHelper.ShowPriceText(equipAdvance.costItemNum,hasItemNum,db.CostItemNum);
        GTItemHelper.ShowItemQuality(equipAdvance.costItemQuality, itemID);
        if (db.CostEquipNum > 0)
        {
            equipAdvance.costEquipBtn.SetActive(true);
            GTItemHelper.ShowItemTexture(equipAdvance.costEquipTexture, itemID);
            GTItemHelper.ShowItemQuality(equipAdvance.costEquipQuality, itemID);
            int hasSameEquipNum = MLEquip.Instance.GetBagSameEquipList(equip).Count;
            GTItemHelper.ShowPriceText(equipAdvance.costEquipNum, hasSameEquipNum, db.CostEquipNum);
        }
        else
        {
            equipAdvance.costEquipBtn.SetActive(false);
        }
        GTItemHelper.ShowItemTexture(equipAdvance.costMoneyTexture, db.CostMoneyId);
        GTItemHelper.ShowItemNum(equipAdvance.costMoneyNum, db.CostMoneyNum);
        GTItemHelper.ShowEquipAdvanceText(equipAdvance.advanceText, itemID, equip.AdvanceLevel);
    }

    private void ShowView()
    {
        equipStrengthen.OnStart(curEquipFunc == EquipFunc.STRENGTHEN);
        equipAdvance.OnStart(curEquipFunc == EquipFunc.ADVANCE);
        equipStar.OnStart(curEquipFunc == EquipFunc.EQUIPSTAR);
        switch (curEquipFunc)
        {
            case EquipFunc.STRENGTHEN:
                currFuncName.text = "装备强化";
                ShowStrengthenView();
                break;
            case EquipFunc.ADVANCE:
                currFuncName.text = "装备进阶";
                ShowAdvanceView();
                break;
            case EquipFunc.EQUIPSTAR:
                currFuncName.text = "装备升星";
                ShowEquipStarView();
                break;
        }
    }

    private void TrySwitchFunc(EquipFunc func)
    {
        if (curEquipFunc == func)
        {
            return;
        }
        curEquipFunc = func;
        curStrengthenCostItems.Clear();
        ShowView();
    }

    private void OnRecvStrengthenEquip()
    {
        curStrengthenCostItems.Clear();
        ShowView();
    }

    private void OnRecvAdvanceEquip()
    {
        ShowView();
    }

    private void OnRecvUpStarEquip()
    {
        ShowView();
    }

    public void ShowEquipWindowByPos(EPosType posType, int pos)
    {
        mPosType = posType;
        mPos = pos;
        ShowView();
    }
}
