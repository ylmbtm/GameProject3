
using UnityEngine;
using System.Collections;
using System;
using Protocol;

public class UIItemInfo : GTWindow
{
    private GameObject btnUse;
    private GameObject btnBatchUse;
    private GameObject btnClose;
    private UILabel itemName;
    private UILabel itemDesc;
    private UILabel itemNum;
    private UITexture itemTexture;
    private UISprite itemQuality;
    private int mPos;
    private Transform pivot;


    public UIItemInfo()
    {
        mResident = true;
        mResPath = "Public/UIItemInfo";
        Type = EWindowType.DIALOG;
    }

    protected override void OnAwake()
    {
        pivot = transform.Find("Pivot");
        btnClose = pivot.Find("Btn_Close").gameObject;
        btnUse = pivot.Find("Btn_Use").gameObject;
        btnBatchUse = pivot.Find("Btn_BatchUse").gameObject;
        itemDesc = pivot.Find("ItemDesc").GetComponent<UILabel>();
        itemName = pivot.Find("ItemName").GetComponent<UILabel>();
        itemNum = pivot.Find("ItemNum").GetComponent<UILabel>();
        itemTexture = pivot.Find("Item/Texture").GetComponent<UITexture>();
        itemQuality = pivot.Find("Item/Quality").GetComponent<UISprite>();
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_USE_ITEM, OnRecvUseItem);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_USE_ITEM, OnRecvUseItem);
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnUse).onClick = OnUseClick;
        UIEventListener.Get(btnBatchUse).onClick = OnBatchUseClick;
    }

    private void OnBatchUseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTItemHelper.ShowItemUseDialogByPos(mPos);
    }

    private void OnUseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        BagService.Instance.TryUseItemByPos(mPos, 1);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    protected override void OnEnable()
    {
       
    }

    private void OnRecvUseItem(int id,int num)
    {
        Hide();
    }

    protected override void OnClose()
    {
        mPos = 0;
    }

    public void ShowInfoById(int itemID)
    {
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        itemDesc.text = itemDB.Desc;
        itemNum.text = string.Empty;
        GTItemHelper.ShowItemTexture(itemTexture, itemID);
        GTItemHelper.ShowItemName(itemName, itemID);
        GTItemHelper.ShowItemQuality(itemQuality, itemID);
        btnBatchUse.SetActive(false);
        btnUse.SetActive(false);
    }


    public void ShowInfoByPos(int pos, bool canUse)
    {
        this.mPos = pos;
        XItem item = DataDBSBagItem.GetDataById(pos);
        int itemID = item.Id;
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        itemDesc.text = itemDB.Desc;
        int hasNum = DataManager.Instance.GetItemCountById(itemID);
        itemNum.text = GTTools.Format("拥有数量：{0}", hasNum);
        GTItemHelper.ShowItemTexture(itemTexture, itemID);
        GTItemHelper.ShowItemName(itemName, itemID);
        GTItemHelper.ShowItemQuality(itemQuality, itemID);
        btnBatchUse.SetActive(canUse);
        btnUse.SetActive(canUse);
    }
}
