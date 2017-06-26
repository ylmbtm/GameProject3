using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIGem : GTWindow
{
    public UIGem()
    {
        mResident = false;
        mResPath = "Gem/UIGem";
        Type = EWindowType.WINDOW;
    }


    protected override void OnAwake()
    {
        labMoneyNum1 = transform.Find("Money_1/Num").GetComponent<UILabel>();
        labMoneyNum2 = transform.Find("Money_2/Num").GetComponent<UILabel>();
        moneyTexture1 = transform.Find("Money_1/Texture").GetComponent<UITexture>();
        moneyTexture2 = transform.Find("Money_2/Texture").GetComponent<UITexture>();
        btnMoneyPlus1 = transform.Find("Money_1/Btn_Plus").gameObject;
        btnMoneyPlus2 = transform.Find("Money_2/Btn_Plus").gameObject;
        btnClose = transform.Find("Btn_Close").gameObject;
        btnReturn= transform.Find("Top/Btn_Return").gameObject;

        Transform pivot = transform.Find("Pivot");
        gemTexture = pivot.Find("Gem/Texture").GetComponent<UITexture>();
        gemQuality = pivot.Find("Gem/Quality").GetComponent<UISprite>();
        costMoneyTexture = pivot.Find("Cost/Money/Texture").GetComponent<UITexture>();
        costMoneyNum = pivot.Find("Cost/Money/Num").GetComponent<UILabel>();
        btnAutoToAddMat = pivot.Find("Btn_AutoToAddMat").gameObject;
        btnStrengthen = pivot.Find("Btn_Strengthen").gameObject;

        gemLevel = pivot.Find("Content0/Level").GetComponent<UILabel>();
        gemExpBar = pivot.Find("Content0").GetComponent<UISlider>();
        gemExpNum = pivot.Find("Content0/Num").GetComponent<UILabel>();
        gemName = pivot.Find("Content1/GemName").GetComponent<UILabel>();
        gemType = pivot.Find("Content1/GemType").GetComponent<UILabel>();
        gemSuit = pivot.Find("Content1/GemSuit").GetComponent<UILabel>();
        gemProperty = pivot.Find("Content2/Text").GetComponent<UILabel>();

        Transform itemsTrans = pivot.Find("Items");
        for (int i = 1; i <= 6; i++)
        {
            ItemStrengthen item = new ItemStrengthen();
            item.itemBtn = itemsTrans.Find("Item_" + i).gameObject;
            item.itemTexture = item.itemBtn.transform.Find("Texture").GetComponent<UITexture>();
            item.itemQuality = item.itemBtn.transform.Find("Quality").GetComponent<UISprite>();
            item.itemNum = item.itemBtn.transform.Find("Num").GetComponent<UILabel>();
            item.itemPlus = item.itemBtn.transform.Find("Plus").gameObject;
            mStrengthenItems.Add(item);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnAutoToAddMat).onClick = OnAutoToAddMatClick;
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnReturn).onClick = OnCloseClick;
        UIEventListener.Get(btnMoneyPlus1).onClick = OnMoneyPlus1Click;
        UIEventListener.Get(btnMoneyPlus2).onClick = OnMoneyPlus2Click;
        UIEventListener.Get(btnStrengthen).onClick = OnStrengthenClick;
    }

    private void OnMoneyPlus2Click(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnMoneyPlus1Click(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }


    private void OnStrengthenClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        XGem gem = DataManager.Instance.GetGemDataByPos(mPosType, mPos);
        GemService.Instance.TryStrengthGem(gem,mStrengthenMats);
    }

    private void OnAutoToAddMatClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        XGem gem = DataManager.Instance.GetGemDataByPos(mPosType, mPos);
        mStrengthenMats = GemModule.Instance.GetItemListToOneKeyStrengthen(gem);
        ShowView();
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_STRENGTHEN_GEM,OnRecvStrengthenGem);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_STRENGTHEN_GEM, OnRecvStrengthenGem);
    }

    private void OnRecvStrengthenGem()
    {
        mStrengthenMats.Clear();
        ShowView();
    }

    protected override void OnEnable()
    {
        
    }


    protected override void OnClose()
    {
        mStrengthenItems.Clear();
        mStrengthenMats.Clear();
    }

    public void ShowWindowByPos(EPosType posType, int pos)
    {
        mPosType = posType;
        mPos = pos;
        ShowView();
    }
    
    private void ShowView()
    {
        labMoneyNum1.text = DataManager.Instance.GetItemCountById(1).ToString();
        labMoneyNum2.text = DataManager.Instance.GetItemCountById(2).ToString();
        GTItemHelper.ShowItemTexture(moneyTexture1, 1);
        GTItemHelper.ShowItemTexture(moneyTexture2, 2);
        XGem gem = DataManager.Instance.GetGemDataByPos(mPosType, mPos);
        int itemID = gem.Id;
        GTItemHelper.ShowItemTexture(gemTexture, itemID);
        GTItemHelper.ShowItemQuality(gemQuality, itemID);
        GTItemHelper.ShowItemName(gemName, itemID);

        int costgemNum = mStrengthenMats.Count;
        for (int i = 0; i < mStrengthenItems.Count; i++)
        {

            ItemStrengthen cell = mStrengthenItems[i];
            if (i < costgemNum)
            {
                XItem mat = mStrengthenMats[i];
                cell.Show(true);
                int id = mStrengthenMats[i].Id;
                GTItemHelper.ShowItemTexture(cell.itemTexture, id);
                GTItemHelper.ShowItemQuality(cell.itemQuality, id);
                GTItemHelper.ShowItemNum(cell.itemNum, mat.Num);
            }
            else
            {
                cell.Show(false);
            }
        }
        if (GemModule.Instance.IsFullStrengthenLevel(gem) == true)
        {
            return;
        }
        DGem gemDB = ReadCfgGem.GetDataById(itemID);
        DGemLevel levelDB = ReadCfgGemLevel.GetDataById(gemDB.Quality*1000+ gem.StrengthenLevel + 1);
        int addExpNum = GemModule.Instance.GetExpByStrengthMatList(mStrengthenMats);
        GTItemHelper.ShowItemTexture(costMoneyTexture, levelDB.CostMoneyId);
        costMoneyNum.text = addExpNum.ToString();
        gemLevel.text = GTTools.Format("强化等级 {0}", gem.StrengthenLevel);
        GTItemHelper.ShowProgressText(gemExpNum, gem.StrengthenExp, levelDB.RequireExp);
        GTItemHelper.ShowProgressSlider(gemExpBar, gem.StrengthenExp, levelDB.RequireExp);
        GTItemHelper.ShowGemPropertyText(gemProperty, itemID, gem.StrengthenLevel, true);

        gemSuit.text = GemModule.Instance.GetGemSuitName(itemID);
        gemType.text = GemModule.Instance.GetGemTypeName(itemID);
    }

    private UILabel labMoneyNum1;
    private UILabel labMoneyNum2;
    private UITexture moneyTexture1;
    private UITexture moneyTexture2;
    private GameObject btnMoneyPlus1;
    private GameObject btnMoneyPlus2;
    private GameObject btnClose;
    private GameObject btnReturn;

    private UITexture gemTexture;
    private UISprite gemQuality;
    private GameObject btnAutoToAddMat;
    private GameObject btnStrengthen;
    private UILabel gemLevel;
    private UISlider gemExpBar;
    private UILabel gemExpNum;
    private UILabel gemName;
    private UILabel gemType;
    private UILabel gemSuit;
    private UILabel gemProperty;
    private UITexture costMoneyTexture;
    private UILabel costMoneyNum;
    private List<ItemStrengthen> mStrengthenItems = new List<ItemStrengthen>();
    private List<XItem> mStrengthenMats = new List<XItem>();


    private int mPos;
    private EPosType mPosType;

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

}
