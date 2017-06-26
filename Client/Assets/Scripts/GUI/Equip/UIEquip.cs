using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIEquip : GTWindow
{
    public UIEquip()
    {
        Type = EWindowType.WINDOW;
        mResident = false;
        mResPath = "Equip/UIEquip";
    }

    protected override void OnAwake()
    {
        labMoneyNum1 = transform.Find("Money_1/Num").GetComponent<UILabel>();
        labMoneyNum2 = transform.Find("Money_2/Num").GetComponent<UILabel>();
        moneyTexture1 = transform.Find("Money_1/Texture").GetComponent<UITexture>();
        moneyTexture2 = transform.Find("Money_2/Texture").GetComponent<UITexture>();
        btnMoneyPlus1 = transform.Find("Money_1/Btn_Plus").gameObject;
        btnMoneyPlus2 = transform.Find("Money_2/Btn_Plus").gameObject;

        menuBtn1 = transform.Find("MenuBtn/MenuBtn_1").gameObject;
        menuBtn2 = transform.Find("MenuBtn/MenuBtn_2").gameObject;
        menuBtn3 = transform.Find("MenuBtn/MenuBtn_3").gameObject;
        btnReturn = transform.Find("Top/Btn_Return").gameObject;
        btnClose = transform.Find("Btn_Close").gameObject;
        currFuncName = transform.Find("Title").GetComponent<UILabel>();

        Transform strengthen = transform.Find("View1");
        mEquipStrengthen.panel = strengthen.gameObject;
        mEquipStrengthen.equipTexture = strengthen.Find("Equip/Texture").GetComponent<UITexture>();
        mEquipStrengthen.equipQuality = strengthen.Find("Equip/Quality").GetComponent<UISprite>();
        mEquipStrengthen.costMoneyTexture = strengthen.Find("Cost/Money/Texture").GetComponent<UITexture>();
        mEquipStrengthen.costMoneyNum = strengthen.Find("Cost/Money/Num").GetComponent<UILabel>();
        mEquipStrengthen.btnAutoToAddMat = strengthen.Find("Btn_AutoToAddMat").gameObject;
        mEquipStrengthen.btnStrengthen = strengthen.Find("Btn_Strengthen").gameObject;
        mEquipStrengthen.btnStrengthenTenTimes = strengthen.Find("Btn_StrengthenTenTimes").gameObject;
        mEquipStrengthen.equipName = strengthen.Find("EquipName").GetComponent<UILabel>();

        mEquipStrengthen.equipStrengthExpBar = strengthen.Find("Content0").GetComponent<UISlider>();
        mEquipStrengthen.equipStrengthLevel = strengthen.Find("Content0/Level").GetComponent<UILabel>();
        mEquipStrengthen.equipStrengthExp = strengthen.Find("Content0/Num").GetComponent<UILabel>();
        mEquipStrengthen.propertyText = strengthen.Find("Content1/Text").GetComponent<UILabel>();
        mEquipStrengthen.strengthText = strengthen.Find("Content2/Text").GetComponent<UILabel>();

        Transform itemsTrans = strengthen.Find("Items");
        for (int i = 1; i <= 6; i++)
        {
            ItemStrengthen item = new ItemStrengthen();
            item.itemBtn = itemsTrans.Find("Item_" + i).gameObject;
            item.itemTexture = item.itemBtn.transform.Find("Texture").GetComponent<UITexture>();
            item.itemQuality = item.itemBtn.transform.Find("Quality").GetComponent<UISprite>();
            item.itemNum = item.itemBtn.transform.Find("Num").GetComponent<UILabel>();
            item.itemPlus = item.itemBtn.transform.Find("Plus").gameObject;
            mEquipStrengthen.mStrengthenItems.Add(item);
        }


        Transform advance = transform.Find("View2");
        mEquipAdvance.panel = advance.gameObject;
        mEquipAdvance.nowEquipTexture = advance.Find("Now/Equip/Texture").GetComponent<UITexture>();
        mEquipAdvance.nowEquipQuality = advance.Find("Now/Equip/Quality").GetComponent<UISprite>();
        mEquipAdvance.nowEquipName = advance.Find("Now/EquipName").GetComponent<UILabel>();
        mEquipAdvance.nowEquipFightValue = advance.Find("Now/FightValue/Num").GetComponent<UILabel>();
        mEquipAdvance.nowEquipPropertys = advance.Find("Now/Propertys").GetComponent<UILabel>();
        mEquipAdvance.nexEquipTexture = advance.Find("Next/Equip/Texture").GetComponent<UITexture>();
        mEquipAdvance.nexEquipQuality = advance.Find("Next/Equip/Quality").GetComponent<UISprite>();
        mEquipAdvance.nexEquipName = advance.Find("Next/EquipName").GetComponent<UILabel>();
        mEquipAdvance.nexEquipFightValue = advance.Find("Next/FightValue/Num").GetComponent<UILabel>();
        mEquipAdvance.nexEquipPropertys = advance.Find("Next/Propertys").GetComponent<UILabel>();
        mEquipAdvance.btnAdvance = advance.Find("Btn_Advance").gameObject;

        mEquipAdvance.costEquipBtn = advance.Find("Cost/Equip").gameObject;
        mEquipAdvance.costEquipNum = advance.Find("Cost/Equip/Num").GetComponent<UILabel>();
        mEquipAdvance.costEquipTexture = advance.Find("Cost/Equip/Texture").GetComponent<UITexture>();
        mEquipAdvance.costEquipQuality = advance.Find("Cost/Equip/Quality").GetComponent<UISprite>();

        mEquipAdvance.costItemBtn = advance.Find("Cost/Item").gameObject;
        mEquipAdvance.costItemNum = advance.Find("Cost/Item/Num").GetComponent<UILabel>();
        mEquipAdvance.costItemTexture = advance.Find("Cost/Item/Texture").GetComponent<UITexture>();
        mEquipAdvance.costItemQuality = advance.Find("Cost/Item/Quality").GetComponent<UISprite>();
        mEquipAdvance.costMoneyNum = advance.Find("Btn_Advance/Num").GetComponent<UILabel>();
        mEquipAdvance.costMoneyTexture=advance.Find("Btn_Advance/Texture").GetComponent<UITexture>();
        mEquipAdvance.advanceText = advance.Find("Content0/Text").GetComponent<UILabel>();



        Transform equipStar = transform.Find("View3");
        mEquipStar.panel = equipStar.gameObject;
        mEquipStar.nowEquipTexture = equipStar.Find("Now/Equip/Texture").GetComponent<UITexture>();
        mEquipStar.nowEquipQuality = equipStar.Find("Now/Equip/Quality").GetComponent<UISprite>();
        mEquipStar.nowEquipName = equipStar.Find("Now/EquipName").GetComponent<UILabel>();
        mEquipStar.nowEquipPropertys = equipStar.Find("Now/Propertys").GetComponent<UILabel>();
        mEquipStar.nexEquipTexture = equipStar.Find("Next/Equip/Texture").GetComponent<UITexture>();
        mEquipStar.nexEquipQuality = equipStar.Find("Next/Equip/Quality").GetComponent<UISprite>();
        mEquipStar.nexEquipName = equipStar.Find("Next/EquipName").GetComponent<UILabel>();
        mEquipStar.nexEquipPropertys = equipStar.Find("Next/Propertys").GetComponent<UILabel>();

        equipStar.Find("Now/Stars").GetComponentsInChildren(mEquipStar.nowEquipStars);
        equipStar.Find("Next/Stars").GetComponentsInChildren(mEquipStar.nexEquipStars);

        mEquipStar.btnUpStar = equipStar.Find("Btn_UpStar").gameObject;
        mEquipStar.costMoneyNum = equipStar.Find("Btn_UpStar/Num").GetComponent<UILabel>();
        mEquipStar.costMoneyTexture=equipStar.Find("Btn_UpStar/Texture").GetComponent<UITexture>();

        mEquipStar.costItemBtn = equipStar.Find("Cost/Item").gameObject;
        mEquipStar.costItemTexture=equipStar.Find("Cost/Item/Texture").GetComponent<UITexture>();
        mEquipStar.costItemQuality=equipStar.Find("Cost/Item/Quality").GetComponent<UISprite>();
        mEquipStar.costItemNum=equipStar.Find("Cost/Item/Num").GetComponent<UILabel>();

        mEquipStar.starText=equipStar.Find("Content0/Text").GetComponent<UILabel>();

    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_STRENGTHEN_EQUIP, OnRecvStrengthenEquip);
        GTEventCenter.AddHandler(GTEventID.TYPE_ADVANCE_EQUIP, OnRecvAdvanceEquip);
        GTEventCenter.AddHandler(GTEventID.TYPE_UPSTAR_EQUIP, OnRecvUpStarEquip);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_STRENGTHEN_EQUIP, OnRecvStrengthenEquip);
        GTEventCenter.DelHandler(GTEventID.TYPE_ADVANCE_EQUIP, OnRecvAdvanceEquip);
        GTEventCenter.DelHandler(GTEventID.TYPE_UPSTAR_EQUIP, OnRecvUpStarEquip);
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnReturn).onClick = OnCloseClick;
        UIEventListener.Get(menuBtn1).onClick = OnStrengthenClick;
        UIEventListener.Get(menuBtn2).onClick = OnAdvanceClick;
        UIEventListener.Get(menuBtn3).onClick = OnEquipStarClick;
        UIEventListener.Get(btnMoneyPlus1).onClick = OnMoneyPlusClick1;
        UIEventListener.Get(btnMoneyPlus2).onClick = OnMoneyPlusClick2;
        UIEventListener.Get(mEquipStrengthen.btnAutoToAddMat).onClick = OnAutoToAddMatClick;
        UIEventListener.Get(mEquipStrengthen.btnStrengthen).onClick = OnStrengthenEquipClick;
        UIEventListener.Get(mEquipAdvance.btnAdvance).onClick = OnAdvanceEquipClick;
        UIEventListener.Get(mEquipStar.btnUpStar).onClick = OnUpStarEquipClick;
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
        if (mStrengthenCostItems.Count>0)
        {
            return;
        }
        XEquip equip = DataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        List<XItem> list = EquipModule.Instance.GetItemListToOneKeyStrengthen(equip);
        if (list.Count == 0)
        {
            GTItemHelper.ShowTip("没有蓝绿装，且没有水晶");
            return;
        }
        mStrengthenCostItems = list;
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
        XEquip equip = DataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        EquipService.Instance.TryStrengthEquip(equip, mStrengthenCostItems);
    }

    private void OnAdvanceEquipClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        XEquip equip = DataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        EquipService.Instance.TryAdvanceEquip(equip);
    }

    private void OnUpStarEquipClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        XEquip equip = DataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        EquipService.Instance.TryUpStarEquip(equip);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    private void ShowStrengthenView()
    {
        XEquip equip = DataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        if (equip == null) return;
        if (EquipModule.Instance.IsFullStrengthenLevel(equip) == true)
        {
            return;
        }
        int itemID = equip.Id;   
        GTItemHelper.ShowItemTexture(mEquipStrengthen.equipTexture, itemID);
        GTItemHelper.ShowItemQuality(mEquipStrengthen.equipQuality, itemID);
        GTItemHelper.ShowItemName(mEquipStrengthen.equipName, itemID);

        int costEquipNum = mStrengthenCostItems.Count;
        for(int i=0;i<mEquipStrengthen.mStrengthenItems.Count;i++)
        {
      
            ItemStrengthen cell = mEquipStrengthen.mStrengthenItems[i];
            if(i<costEquipNum)
            {
                XItem mat = mStrengthenCostItems[i];
                cell.Show(true);
                int id = mStrengthenCostItems[i].Id;
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
        int addExpNum = EquipModule.Instance.GetStrengthenExpByMatList(mStrengthenCostItems);
        GTItemHelper.ShowItemTexture(mEquipStrengthen.costMoneyTexture, DEquip.EQUIP_STRENGTHEN_MONEY_ID_1);
        mEquipStrengthen.costMoneyNum.text = addExpNum.ToString();
        mEquipStrengthen.equipStrengthLevel.text = GTTools.Format("强化等级 {0}", equip.StrengthenLevel);
        GTItemHelper.ShowProgressText(mEquipStrengthen.equipStrengthExp, equip.StrengthenExp, levelDB.RequireExp);
        GTItemHelper.ShowProgressSlider(mEquipStrengthen.equipStrengthExpBar, equip.StrengthenExp, levelDB.RequireExp);
        Dictionary<EAttr, int> propertys = AttrHelper.GetPropertys(equip);
        GTItemHelper.ShowEquipStrengthText(mEquipStrengthen.strengthText, itemID, equip.StrengthenLevel);
        GTItemHelper.ShowPropertyText(mEquipStrengthen.propertyText, propertys, true);
    }

    private void ShowEquipStarView()
    {
        XEquip equip = DataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        if (equip == null) return;
        int itemID = equip.Id;
        if (EquipModule.Instance.IsFullAdvanceLevel(equip) == true)
        {
            return;
        }
        GTItemHelper.ShowItemTexture(mEquipStar.nowEquipTexture, itemID);
        GTItemHelper.ShowItemQuality(mEquipStar.nowEquipQuality, itemID);
        GTItemHelper.ShowItemName(mEquipStar.nowEquipName, itemID);
        GTItemHelper.ShowItemTexture(mEquipStar.nexEquipTexture, itemID);
        GTItemHelper.ShowItemQuality(mEquipStar.nexEquipQuality, itemID);
        GTItemHelper.ShowItemName(mEquipStar.nexEquipName, itemID);
        GTItemHelper.ShowStarNum(mEquipStar.nowEquipStars, equip.StarLevel);
        GTItemHelper.ShowStarNum(mEquipStar.nexEquipStars, equip.StarLevel + 1);

        DEquip equipDB = ReadCfgEquip.GetDataById(equip.Id);
        DEquipStar nexStarDB = ReadCfgEquipStar.GetDataById(equipDB.Quality * 1000 + equip.StarLevel + 1);

        int hasItemNum = DataManager.Instance.GetItemCountById(nexStarDB.CostItemId);
        GTItemHelper.ShowItemTexture(mEquipStar.costItemTexture, nexStarDB.CostItemId);
        GTItemHelper.ShowItemQuality(mEquipStar.costItemQuality, nexStarDB.CostItemId);
        GTItemHelper.ShowPriceText(mEquipStar.costItemNum, hasItemNum, nexStarDB.CostItemNum);

        GTItemHelper.ShowItemTexture(mEquipStar.costMoneyTexture, nexStarDB.CostMoneyId);
        mEquipStar.costMoneyNum.text = nexStarDB.CostMoneyNum.ToString();
        GTItemHelper.ShowEquipStarText(mEquipStar.starText, itemID, equip.StarLevel);
        GTItemHelper.ShowEquipStarText(mEquipStar.nowEquipPropertys, itemID, equip.StarLevel, false);
        GTItemHelper.ShowEquipStarText(mEquipStar.nexEquipPropertys, itemID, equip.StarLevel + 1, false);
    }

    private void ShowAdvanceView()
    {
        XEquip equip = DataManager.Instance.GetEquipDataByPos(mPosType, mPos);
        if (equip == null) return;
        int itemID = equip.Id;
        if(EquipModule.Instance.IsFullStarLevel(equip))
        {
            return;
        }
        GTItemHelper.ShowItemTexture(mEquipAdvance.nowEquipTexture, itemID);
        GTItemHelper.ShowItemQuality(mEquipAdvance.nowEquipQuality, itemID);
        GTItemHelper.ShowItemName(mEquipAdvance.nowEquipName, itemID);

        GTItemHelper.ShowItemTexture(mEquipAdvance.nexEquipTexture, itemID);
        GTItemHelper.ShowItemQuality(mEquipAdvance.nexEquipQuality, itemID);
        GTItemHelper.ShowItemName(mEquipAdvance.nexEquipName, itemID);

        Dictionary<EAttr, int> nowPropertys = AttrHelper.GetPropertys(equip);
        XEquip nexEquip = new XEquip();
        GTTools.Copy(nexEquip, equip);
        nexEquip.AdvanceLevel++;
        Dictionary<EAttr, int> nexPropertys = AttrHelper.GetPropertys(nexEquip);

        GTItemHelper.ShowPropertyText(mEquipAdvance.nowEquipPropertys, nowPropertys, false);
        GTItemHelper.ShowPropertyText(mEquipAdvance.nexEquipPropertys, nexPropertys, false);
        mEquipAdvance.nowEquipFightValue.text = AttrHelper.GetFightValue(nowPropertys).ToString();
        mEquipAdvance.nexEquipFightValue.text = AttrHelper.GetFightValue(nexPropertys).ToString();

        DEquip equipDB = ReadCfgEquip.GetDataById(itemID);
        DEquipAdvanceCost db = ReadCfgEquipAdvanceCost.GetDataById(equipDB.Quality*1000+ equip.AdvanceLevel + 1);
        int hasItemNum = DataManager.Instance.GetItemCountById(db.CostItemId);
        GTItemHelper.ShowItemTexture(mEquipAdvance.costItemTexture, db.CostItemId);
        GTItemHelper.ShowPriceText(mEquipAdvance.costItemNum,hasItemNum,db.CostItemNum);
        GTItemHelper.ShowItemQuality(mEquipAdvance.costItemQuality, itemID);
        if(db.CostEquipNum>0)
        {
            mEquipAdvance.costEquipBtn.SetActive(true);
            GTItemHelper.ShowItemTexture(mEquipAdvance.costEquipTexture,itemID);
            GTItemHelper.ShowItemQuality(mEquipAdvance.costEquipQuality, itemID);
            int hasSameEquipNum = EquipModule.Instance.GetBagSameEquipList(equip).Count;
            GTItemHelper.ShowPriceText(mEquipAdvance.costEquipNum, hasSameEquipNum, db.CostEquipNum);
        }
        else
        {
            mEquipAdvance.costEquipBtn.SetActive(false);
        }
        GTItemHelper.ShowItemTexture(mEquipAdvance.costMoneyTexture, db.CostMoneyId);
        GTItemHelper.ShowItemNum(mEquipAdvance.costMoneyNum, db.CostMoneyNum);
        GTItemHelper.ShowEquipAdvanceText(mEquipAdvance.advanceText, itemID, equip.AdvanceLevel);
    }

    private void ShowView()
    {
        mEquipStrengthen.Show(mCurEquipFunc == EquipFunc.STRENGTHEN);
        mEquipAdvance.Show(mCurEquipFunc == EquipFunc.ADVANCE);
        mEquipStar.Show(mCurEquipFunc == EquipFunc.EQUIPSTAR);
        switch (mCurEquipFunc)
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

        labMoneyNum1.text = DataManager.Instance.GetItemCountById(1).ToString();
        labMoneyNum2.text = DataManager.Instance.GetItemCountById(2).ToString();
        GTItemHelper.ShowItemTexture(moneyTexture1,1);
        GTItemHelper.ShowItemTexture(moneyTexture2,2);
    }

    private void TrySwitchFunc(EquipFunc func)
    {
        if(mCurEquipFunc==func)
        {
            return;
        }
        mCurEquipFunc = func;
        mStrengthenCostItems.Clear();
        ShowView();
    }

    protected override void OnEnable()
    {
        InitMenuBtns();
    }

    protected override void OnClose()
    {
        mCurEquipFunc = EquipFunc.STRENGTHEN;
        mStrengthenCostItems.Clear();
        mEquipStrengthen.Release();
        mEquipAdvance.Release();
        mEquipStar.Release();
    }

    private void OnRecvStrengthenEquip()
    {
        mStrengthenCostItems.Clear();
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


    private UILabel labMoneyNum1;
    private UILabel labMoneyNum2;
    private UITexture moneyTexture1;
    private UITexture moneyTexture2;
    private GameObject btnMoneyPlus1;
    private GameObject btnMoneyPlus2;

    private GameObject menuBtn1;
    private GameObject menuBtn2;
    private GameObject menuBtn3;
    private GameObject btnClose;
    private GameObject btnReturn;
    private UILabel currFuncName;

    private EquipStrengthen mEquipStrengthen = new EquipStrengthen();
    private EquipAdvance mEquipAdvance = new EquipAdvance();
    private EquipStar mEquipStar = new EquipStar();

    private EquipFunc mCurEquipFunc = EquipFunc.STRENGTHEN;
    private List<XItem> mStrengthenCostItems = new List<XItem>();


    private int mPos;
    private EPosType mPosType;

    enum EquipFunc
    {
        STRENGTHEN,
        ADVANCE,
        EQUIPSTAR,
    }

    class ItemStrengthen
    {
        public GameObject itemBtn;
        public UITexture itemTexture;
        public UISprite itemQuality;
        public GameObject itemPlus;
        public UILabel itemNum;

        public void Show(bool isFilled)
        {
            itemTexture.gameObject.SetActive(isFilled);
            itemQuality.gameObject.SetActive(isFilled);
            itemNum.gameObject.SetActive(isFilled);
            itemPlus.gameObject.SetActive(!isFilled);
        }
    }

    class EquipStrengthen
    {
        public GameObject panel;
        public UITexture equipTexture;
        public UISprite equipQuality;
        public GameObject btnAutoToAddMat;
        public GameObject btnStrengthen;
        public GameObject btnStrengthenTenTimes;
        public UILabel  equipStrengthLevel;
        public UISlider equipStrengthExpBar;
        public UILabel  equipStrengthExp;
        public UILabel equipName;
        public UILabel propertyText;
        public UILabel strengthText;
        public UITexture costMoneyTexture;
        public UILabel costMoneyNum;
        public List<ItemStrengthen> mStrengthenItems = new List<ItemStrengthen>();

        public void Release()
        {
            mStrengthenItems.Clear();
        }

        public void Show(bool isShow)
        {
            panel.SetActive(isShow);
        }
    }

    class EquipAdvance
    {
        public GameObject panel;
        public GameObject btnAdvance;
        public UILabel costMoneyNum;
        public UITexture costMoneyTexture;
        public UILabel advanceText;

        public UITexture nowEquipTexture;
        public UITexture nexEquipTexture;
        public UISprite nowEquipQuality;
        public UISprite nexEquipQuality;
        public UILabel nowEquipName;
        public UILabel nexEquipName;
        public UILabel nowEquipFightValue;
        public UILabel nexEquipFightValue;
        public UILabel nowEquipPropertys;
        public UILabel nexEquipPropertys;

  

        public GameObject costItemBtn;
        public UITexture costItemTexture;
        public UISprite  costItemQuality;
        public UILabel   costItemNum;

        public GameObject costEquipBtn;
        public UITexture costEquipTexture;
        public UISprite  costEquipQuality;
        public UILabel   costEquipNum;

        public void Show(bool isShow)
        {
            panel.SetActive(isShow);
        }

        public void Release()
        {

        }
    }

    class EquipStar
    {
        public GameObject panel;
        public UILabel starText;
        public GameObject btnUpStar;
        public UILabel   costMoneyNum;
        public UITexture costMoneyTexture;

        public UITexture nowEquipTexture;
        public UITexture nexEquipTexture;
        public UISprite nowEquipQuality;
        public UISprite nexEquipQuality;
        public UILabel nowEquipName;
        public UILabel nexEquipName;
        public UILabel nowEquipPropertys;
        public UILabel nexEquipPropertys;

        public List<UISprite> nowEquipStars = new List<UISprite>();
        public List<UISprite> nexEquipStars = new List<UISprite>();

        public GameObject costItemBtn;
        public UITexture  costItemTexture;
        public UISprite   costItemQuality;
        public UILabel    costItemNum;

        public void Release()
        {
            nowEquipStars.Clear();
            nexEquipStars.Clear();
        }

        public void Show(bool isShow)
        {
            panel.SetActive(isShow);
        }
    }
}
