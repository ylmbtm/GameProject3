using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIGem : GTWindow
{
    private GameObject                btnClose;

    private UITexture                 gemTexture;
    private UISprite                  gemQuality;
    private GameObject                btnAutoToAddMat;
    private GameObject                btnStrengthen;
    private UILabel                   gemLevel;
    private UISlider                  gemExpBar;
    private UILabel                   gemExpNum;
    private UILabel                   gemName;
    private UILabel                   gemType;
    private UILabel                   gemSuit;
    private UILabel                   gemProperty;
    private UITexture                 costMoneyTexture;
    private UILabel                   costMoneyNum;

    private List<UCGemItemStrengthen> mStrengthenItems = new List<UCGemItemStrengthen>();
    private List<XItem>               mStrengthenMats  = new List<XItem>();
    private int                       mPos;
    private EPosType                  mPosType;

    public UIGem()
    {
        Resident = false;
        Path = "Gem/UIGem";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.Black;
        ShowMode = EWindowShowMode.HideOther;
    }

    protected override void OnAwake()
    {
    
        Transform pivot = transform.Find("Pivot");
        btnClose = pivot.Find("BtnClose").gameObject;
        gemTexture = pivot.Find("Gem/Texture").GetComponent<UITexture>();
        gemQuality = pivot.Find("Gem/Quality").GetComponent<UISprite>();
        costMoneyTexture = pivot.Find("Cost/Texture").GetComponent<UITexture>();
        costMoneyNum = pivot.Find("Cost/Num").GetComponent<UILabel>();
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
            UCGemItemStrengthen item = new UCGemItemStrengthen();
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
        UIEventListener.Get(btnStrengthen).onClick = OnStrengthenClick;
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_BAG_STRENGTHEN_GEM, OnRecvStrengthenGem);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_BAG_STRENGTHEN_GEM, OnRecvStrengthenGem);
    }

    protected override void OnEnable()
    {

    }

    protected override void OnClose()
    {
        mStrengthenItems.Clear();
        mStrengthenMats.Clear();
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
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void OnStrengthenClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        XGem gem = GTDataManager.Instance.GetGemDataByPos(mPosType, mPos);
        GTNetworkSend.Instance.TryStrengthGem(gem,mStrengthenMats);
    }

    private void OnAutoToAddMatClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        XGem gem = GTDataManager.Instance.GetGemDataByPos(mPosType, mPos);
        mStrengthenMats = MLGem.Instance.GetItemListToOneKeyStrengthen(gem);
        ShowView();
    }

    private void OnRecvStrengthenGem()
    {
        mStrengthenMats.Clear();
        ShowView();
    }

    private void ShowView()
    {
        XGem gem = GTDataManager.Instance.GetGemDataByPos(mPosType, mPos);
        int itemID = gem.Id;
        GTItemHelper.ShowItemTexture(gemTexture, itemID);
        GTItemHelper.ShowItemQuality(gemQuality, itemID);
        GTItemHelper.ShowItemName(gemName, itemID);

        int costgemNum = mStrengthenMats.Count;
        for (int i = 0; i < mStrengthenItems.Count; i++)
        {

            UCGemItemStrengthen cell = mStrengthenItems[i];
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
        if (MLGem.Instance.IsFullStrengthenLevel(gem) == true)
        {
            return;
        }
        DGem gemDB = ReadCfgGem.GetDataById(itemID);
        DGemLevel levelDB = ReadCfgGemLevel.GetDataById(gemDB.Quality * 1000 + gem.StrengthenLevel + 1);
        int addExpNum = MLGem.Instance.GetExpByStrengthMatList(mStrengthenMats);
        GTItemHelper.ShowItemTexture(costMoneyTexture, levelDB.CostMoneyId);
        costMoneyNum.text = addExpNum.ToString();
        gemLevel.text = GTTools.Format("强化等级 {0}", gem.StrengthenLevel);
        GTItemHelper.ShowProgressText(gemExpNum, gem.StrengthenExp, levelDB.RequireExp);
        GTItemHelper.ShowProgressSlider(gemExpBar, gem.StrengthenExp, levelDB.RequireExp);
        GTItemHelper.ShowGemPropertyText(gemProperty, itemID, gem.StrengthenLevel, true);
        gemSuit.text = MLGem.Instance.GetGemSuitName(itemID);
        gemType.text = MLGem.Instance.GetGemTypeName(itemID);
    }

    public void  ShowWindowByPos(EPosType posType, int pos)
    {
        mPosType = posType;
        mPos = pos;
        ShowView();
    }
}
