using UnityEngine;
using System.Collections;
using System;
using Protocol;

public class UIItemUse : GTWindow
{
    private GameObject btnClose;
    private GameObject btnCancel;
    private GameObject btnSure;
    private GameObject btnDisTen;
    private GameObject btnDisOne;
    private GameObject btnPlusTen;
    private GameObject btnPlusOne;

    private UILabel   itemCurrCount;
    private UILabel   itemName;
    private UITexture itemTexture;
    private UISprite  itemQuality;
    private UILabel   itemNum;
    private int       mCurrCount;
    private int       mMaxUseCount;
    private int       mItemPos;

    public UIItemUse()
    {
        Type = EWindowType.Window;
        Resident = false;
        Path = "Public/UIItemUse";
        MaskType = EWindowMaskType.BlackTransparent;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnClose = pivot.Find("BtnClose").gameObject;
        btnSure = pivot.Find("BtnSure").gameObject;
        btnCancel = pivot.Find("BtnCancel").gameObject;
        btnPlusOne = pivot.Find("BtnPlusOne").gameObject;
        btnPlusTen = pivot.Find("BtnPlusTen").gameObject;
        btnDisTen = pivot.Find("BtnDisTen").gameObject;
        btnDisOne = pivot.Find("BtnDisOne").gameObject;

        itemCurrCount = pivot.Find("CurrCount").GetComponent<UILabel>();
        itemName = pivot.Find("Item/Name").GetComponent<UILabel>();
        itemQuality = pivot.Find("Item/Quality").GetComponent<UISprite>();
        itemTexture = pivot.Find("Item/Texture").GetComponent<UITexture>();
        itemNum = pivot.Find("Item/Num").GetComponent<UILabel>();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnCancel).onClick = OnCancelClick;
        UIEventListener.Get(btnDisOne).onClick = OnDisOneClick;
        UIEventListener.Get(btnDisTen).onClick = OnDisTenClick;
        UIEventListener.Get(btnPlusOne).onClick = OnPlusOneClick;
        UIEventListener.Get(btnPlusTen).onClick = OnPlusTenClick;
        UIEventListener.Get(btnSure).onClick = OnSureClick;
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_USE_ITEM, OnRecvUseDrug);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_USE_ITEM, OnRecvUseDrug);
    }

    protected override void OnEnable()
    {
        
    }

    protected override void OnClose()
    {
       
    }

    private void OnRecvUseDrug(int id, int num)
    {
        Hide();
    }

    private void OnSureClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryUseItemByPos(mItemPos, mCurrCount);
    }

    private void OnPlusTenClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        int showNum = mMaxUseCount - mCurrCount > 10 ? mCurrCount + 10 : mMaxUseCount;
        SetCurrCount(showNum);
    }

    private void OnPlusOneClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        int showNum = mMaxUseCount - mCurrCount > 1 ? mCurrCount + 1 : mMaxUseCount;
        SetCurrCount(showNum);
    }

    private void OnDisTenClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        int showNum = mCurrCount - 10 > 1 ? mCurrCount - 10 : 1;
        SetCurrCount(showNum);
    }

    private void OnDisOneClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        int showNum = mCurrCount - 1 > 1 ? mCurrCount - 1 : 1;
        SetCurrCount(showNum);
    }

    private void OnCancelClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void SetCurrCount(int count)
    {
        mCurrCount = count;
        itemCurrCount.text = mCurrCount.ToString();
    }

    public void ShowView(int pos)
    {
        mItemPos = pos;
        XItem item = DataDBSBagItem.GetDataById(pos);
        mMaxUseCount = GTDataManager.Instance.GetItemCountById(item.Id);

        GTItemHelper.ShowItemName(itemName, item.Id);
        GTItemHelper.ShowItemTexture(itemTexture, item.Id);
        GTItemHelper.ShowItemQuality(itemQuality, item.Id);
        SetCurrCount(1);
        itemNum.text = GTTools.Format("拥有数量：{0}", item.Num);
    }
}
