
using UnityEngine;
using System.Collections;
using System;
using Protocol;

public class UIItemInfo : GTWindow
{
    private GameObject btnUse;
    private GameObject btnBatchUse;
    private GameObject btnClose;
    private GameObject btnSure;
    private UILabel    itemName;
    private UILabel    itemDesc;
    private UILabel    itemNum;
    private UITexture  itemTexture;
    private UISprite   itemQuality;
    private int        pos;
    private Transform  pivot;

    public UIItemInfo()
    {
        Resident = true;
        Path = "Public/UIItemInfo";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
    }

    protected override void OnAwake()
    {
        pivot = transform.Find("Pivot");
        btnClose = pivot.Find("BtnClose").gameObject;
        btnUse = pivot.Find("BtnUse").gameObject;
        btnBatchUse = pivot.Find("BtnBatchUse").gameObject;
        btnSure = pivot.Find("BtnSure").gameObject;
        itemDesc = pivot.Find("ItemDesc").GetComponent<UILabel>();
        itemName = pivot.Find("Item/Name").GetComponent<UILabel>();
        itemNum = pivot.Find("Item/Num").GetComponent<UILabel>();
        itemTexture = pivot.Find("Item/Texture").GetComponent<UITexture>();
        itemQuality = pivot.Find("Item/Quality").GetComponent<UISprite>();
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_USE_ITEM, OnRecvUseItem);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_USE_ITEM, OnRecvUseItem);
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnUse).onClick = OnUseClick;
        UIEventListener.Get(btnBatchUse).onClick = OnBatchUseClick;
        UIEventListener.Get(btnSure).onClick = OnSureClick;
    }

    protected override void OnClose()
    {
        pos = 0;
    }

    protected override void OnEnable()
    {

    }

    private void OnBatchUseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTItemHelper.ShowItemUseDialogByPos(pos);
    }

    private void OnUseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryUseItemByPos(pos, 1);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void OnSureClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void OnRecvUseItem(int id,int num)
    {
        Hide();
    }

    public void ShowViewById(int itemID)
    {
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        itemDesc.text = itemDB.Desc;
        itemNum.text = string.Empty;
        GTItemHelper.ShowItemTexture(itemTexture, itemID);
        GTItemHelper.ShowItemName(itemName, itemID);
        GTItemHelper.ShowItemQuality(itemQuality, itemID);
        btnBatchUse.SetActive(false);
        btnUse.SetActive(false);
        btnSure.SetActive(true);
    }

    public void ShowViewByPos(int pos, bool canUse)
    {
        this.pos = pos;
        XItem item = DataDBSBagItem.GetDataById(pos);
        int itemID = item.Id;
        DItem itemDB = ReadCfgItem.GetDataById(itemID);
        itemDesc.text = itemDB.Desc;
        itemNum.text = GTTools.Format("拥有数量：{0}", item.Num);
        GTItemHelper.ShowItemTexture(itemTexture, itemID);
        GTItemHelper.ShowItemName(itemName, itemID);
        GTItemHelper.ShowItemQuality(itemQuality, itemID);
        btnBatchUse.SetActive(canUse);
        btnUse.SetActive(canUse);
        btnSure.SetActive(!canUse);
    }
}
