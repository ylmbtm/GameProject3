using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class UIAwardBox : GTWindow
{
    private GameObject                   btnReceive;
    private GameObject                   btnClose;
    private List<Item>                   items = new List<Item>();
    private UILabel                      title;
    private UIEventListener.VoidDelegate DoReceive;

    public UIAwardBox()
    {
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
        Resident = false;
        Path = "Common/UIAwardBox";
        ShowMode = EWindowShowMode.DoNothing;
    }

    public class Item
    {
        public UITexture  itemTexture;
        public UISprite   itemQuality;
        public UILabel    itemNum;
        public UILabel    itemName;
        public GameObject itemBtn;
        public GameObject chip;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnClose = pivot.Find("BtnClose").gameObject;
        btnReceive = pivot.Find("BtnReceive").gameObject;
        title = pivot.Find("Title").GetComponent<UILabel>();
        Transform trans = pivot.Find("View/Grid");
        for (int i = 1; i <= 5; i++)
        {
            Item it = new Item();
            it.itemBtn = trans.Find("Item_" + i.ToString()).gameObject;
            it.itemTexture = it.itemBtn.transform.Find("Texture").GetComponent<UITexture>();
            it.itemQuality = it.itemBtn.transform.Find("Quality").GetComponent<UISprite>();
            it.itemNum = it.itemBtn.transform.Find("Num").GetComponent<UILabel>();
            it.itemName = it.itemBtn.transform.Find("Name").GetComponent<UILabel>();
            it.chip = it.itemBtn.transform.Find("Chip").gameObject;
            items.Add(it);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnReceive).onClick = OnReceiveClick;
    }

    protected override void OnAddHandler()
    {
        
    }

    protected override void OnDelHandler()
    {

    }

    protected override void OnEnable()
    {
        
    }

    protected override void OnClose()
    {
        items.Clear();
    }

    private void OnReceiveClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (DoReceive != null)
        {
            DoReceive(go);
            Hide();
        }
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    public void ShowAwardById(int award, ERewardState rewardState, UIEventListener.VoidDelegate doReceive)
    {
        List<GTItem> list = GTAwardHelper.GetAwardDataByID(award);
        DAward db = ReadCfgAward.GetDataById(award);
        for (int i = 0; i < items.Count; i++)
        {
            Item it = items[i];
            if (i < list.Count)
            {
                GTItem vo = list[i];
                it.itemBtn.SetActive(true);
                it.itemNum.text = vo.Num > 1 ? vo.Num.ToString() : string.Empty;
                GTItemHelper.ShowItemTexture(it.itemTexture, vo.Id);
                GTItemHelper.ShowItemQuality(it.itemQuality, vo.Id);
                GTItemHelper.ShowItemName(it.itemName, vo.Id);
                DItem itemDB = ReadCfgItem.GetDataById(vo.Id);
                it.chip.SetActive(itemDB.ItemType == EItemType.CHIP);
            }
            else
            {
                it.itemBtn.SetActive(false);
            }
        }

        switch (rewardState)
        {
            case ERewardState.NOT_RECEIVE:
                btnReceive.GetComponent<UISprite>().spriteName = "67";
                btnReceive.transform.Find("Label").GetComponent<UILabel>().text = "领取";
                break;
            case ERewardState.CAN_RECEIVE:
                btnReceive.GetComponent<UISprite>().spriteName = "8";
                btnReceive.transform.Find("Label").GetComponent<UILabel>().text = "领取";
                break;
            case ERewardState.HAS_RECEIVE:
                btnReceive.GetComponent<UISprite>().spriteName = "9";
                btnReceive.transform.Find("Label").GetComponent<UILabel>().text = "已领取";
                break;
        }
        title.text = db.Name;
        DoReceive = doReceive;
    }
}
