using UnityEngine;
using System.Collections;
using System;
using Protocol;

public class UIChipInfo : GTWindow
{
    private GameObject btnTargetInfo;
    private GameObject btnCheckSource;
    private GameObject btnCompose;
    private GameObject btnClose;
    private UILabel    itemName;
    private UILabel    itemDesc;
    private UILabel    itemNum;
    private UITexture  itemTexture;
    private UISprite   itemQuality;
    private int        mPos;
    private Transform  pivot;


    public UIChipInfo()
    {
        mResident = false;
        mResPath = "Public/UIChipInfo";
        Type = EWindowType.DIALOG;
    }

    protected override void OnAwake()
    {
        pivot = transform.Find("Pivot");
        btnClose = pivot.Find("Btn_Close").gameObject;
        btnTargetInfo = pivot.Find("Btn_TargetInfo").gameObject;
        btnCheckSource = pivot.Find("Btn_CheckSource").gameObject;
        btnCompose = pivot.Find("Btn_Compose").gameObject;
        itemDesc = pivot.Find("ItemDesc").GetComponent<UILabel>();
        itemName = pivot.Find("ItemName").GetComponent<UILabel>();
        itemNum = pivot.Find("ItemNum").GetComponent<UILabel>();
        itemTexture = pivot.Find("Item/Texture").GetComponent<UITexture>();
        itemQuality = pivot.Find("Item/Quality").GetComponent<UISprite>();
    }


    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnTargetInfo).onClick = OnTargetInfoClick;
        UIEventListener.Get(btnCheckSource).onClick = OnCheckSourceClick;
        UIEventListener.Get(btnCompose).onClick = OnComposeClick;
    }


    private void OnComposeClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        BagService.Instance.TryComposeChip(mPos);
    }

    private void OnCheckSourceClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnTargetInfoClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int>(GTEventID.TYPE_COMPOSE_CHIP, OnRecvComposeChip);
    }


    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int>(GTEventID.TYPE_COMPOSE_CHIP, OnRecvComposeChip);
    }


    protected override void OnEnable()
    {

    }

    protected override void OnClose()
    {
       
    }

    public void ShowInfoById(int itemID)
    {
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        itemDesc.text = itemDB.Desc;
        itemNum.text = string.Empty;
        GTItemHelper.ShowItemTexture(itemTexture, itemID);
        GTItemHelper.ShowItemName(itemName, itemID);
        GTItemHelper.ShowItemQuality(itemQuality, itemID);
        btnCheckSource.SetActive(false);
        btnTargetInfo.SetActive(false);
        btnCompose.SetActive(false);
    }


    public void ShowInfoByPos(int pos)
    {
        mPos = pos;
        XItem item = DataDBSBagChip.Dict[pos];
        int itemID = item.Id;
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        itemDesc.text = itemDB.Desc;
        int hasNum = DataManager.Instance.GetItemCountById(itemID);
        itemNum.text = GTTools.Format("拥有数量：{0}", hasNum);
        GTItemHelper.ShowItemTexture(itemTexture, itemID);
        GTItemHelper.ShowItemName(itemName, itemID);
        GTItemHelper.ShowItemQuality(itemQuality, itemID);
        bool canCompose = item.Num >= itemDB.Data1;
        btnCheckSource.SetActive(!canCompose);
        btnTargetInfo.SetActive(!canCompose);
        btnCompose.SetActive(canCompose);
    }

    private void OnRecvComposeChip(int arg1)
    {
        Hide();
    }

}
