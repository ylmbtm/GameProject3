using UnityEngine;
using System.Collections;
using System;
using Protocol;

public class UIItemUse : GTWindow
{

    public UIItemUse()
    {
        Type = EWindowType.DIALOG;
        mResident = false;
        mResPath = "Public/UIItemUse";
    }


    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnClose = pivot.Find("Btn_Close").gameObject;
        btnSure = pivot.Find("Btn_Sure").gameObject;
        btnCancle = pivot.Find("Btn_Cancle").gameObject;
        btnPlusOne = pivot.Find("Btn_PlusOne").gameObject;
        btnPlusTen = pivot.Find("Btn_PlusTen").gameObject;
        btnDisTen = pivot.Find("Btn_DisTen").gameObject;
        btnDisOne = pivot.Find("Btn_DisOne").gameObject;

        itemCurrCount = pivot.Find("CurrCount").GetComponent<UILabel>();
        itemName= pivot.Find("Item/Name").GetComponent<UILabel>();
        itemQuality = pivot.Find("Item/Quality").GetComponent<UISprite>();
        itemTexture = pivot.Find("Item/Texture").GetComponent<UITexture>();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnCancle).onClick = OnCancleClick;
        UIEventListener.Get(btnDisOne).onClick = OnDisOneClick;
        UIEventListener.Get(btnDisTen).onClick = OnDisTenClick;
        UIEventListener.Get(btnPlusOne).onClick = OnPlusOneClick;
        UIEventListener.Get(btnPlusTen).onClick = OnPlusTenClick;
        UIEventListener.Get(btnSure).onClick = OnSureClick;
    }


    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_USE_ITEM, OnRecvUseDrug);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_USE_ITEM, OnRecvUseDrug);
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
        BagService.Instance.TryUseItemByPos(mItemPos, mCurrCount);
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

    private void OnCancleClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    public void ShowInfo(int pos)
    {
        XItem item = DataDBSBagItem.GetDataById(pos);
        GTItemHelper.ShowItemName(itemName, item.Id);
        GTItemHelper.ShowItemTexture(itemTexture, item.Id);
        GTItemHelper.ShowItemQuality(itemQuality, item.Id);
        mItemPos = pos;
        mMaxUseCount = DataManager.Instance.GetItemCountById(item.Id);
        SetCurrCount(1);
    }

    private void SetCurrCount(int count)
    {
        mCurrCount = count;
        itemCurrCount.text = mCurrCount.ToString();
    }

    private GameObject btnClose;
    private GameObject btnCancle;
    private GameObject btnSure;
    private GameObject btnDisTen;
    private GameObject btnDisOne;
    private GameObject btnPlusTen;
    private GameObject btnPlusOne;

    private UILabel itemCurrCount;
    private UILabel itemName;

    private UITexture itemTexture;
    private UISprite itemQuality;

    private int mCurrCount;
    private int mMaxUseCount;
    private int mItemPos;
}
