using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class UIStore : GTWindow
{
    private GameObject        btnClose;
    private UIScrollView      bottomView;
    private UIScrollView      centerView;
    private GameObject        bottomTemp;
    private GameObject        centerTemp;
    private UIGrid            bottomGrid;
    private UIGrid            centerGrid;

    private List<StoreItem>   mStoreList = new List<StoreItem>();
    private int               mStoreType = 1;

    class StoreItem
    {
        public GameObject    obj;
        public GameObject    itemBtn;
        public UITexture     itemTexture;
        public UISprite      itemQuality;
        public UILabel       itemNum;
        public UILabel       itemName;
        public GameObject    btnBuy;
        public UITexture     costMoneyTexture;
        public UILabel       costMoneyNum;
    }

    public UIStore()
    {
        Path = "Store/UIStore";
        Resident = false;
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.Black;
        ShowMode = EWindowShowMode.HideOther;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnClose   = pivot.Find("BtnClose").gameObject;
        bottomView = pivot.Find("Bottom/View").GetComponent<UIScrollView>();
        centerView = pivot.Find("Center/View").GetComponent<UIScrollView>();
        bottomTemp = pivot.Find("Bottom/View/Template").gameObject;
        centerTemp = pivot.Find("Center/View/Template").gameObject;
        bottomGrid = pivot.Find("Bottom/View/Grid").GetComponent<UIGrid>();
        centerGrid = pivot.Find("Center/View/Grid").GetComponent<UIGrid>();
        bottomTemp.SetActive(false);
        centerTemp.SetActive(false);
        CreateCenterItems();
        CreateBottomItems();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnReturnClick;
    }

    protected override void OnAddHandler()
    {

    }

    protected override void OnEnable()
    {
        MLStore.Instance.Clear();
        RefreshView();
    }

    protected override void OnDelHandler()
    {

    }

    protected override void OnClose()
    {
        mStoreList.Clear();
        mStoreType = 1;
    }

    private void OnReturnClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void CreateCenterItems()
    {
        for (int i = 0; i < 40; i++)
        {
            GameObject item = NGUITools.AddChild(centerGrid.gameObject, centerTemp);
            StoreItem tab = new StoreItem();
            tab.obj = item;
            tab.itemBtn = item.transform.Find("Item").gameObject;
            tab.itemTexture = tab.itemBtn.transform.Find("Texture").GetComponent<UITexture>();
            tab.itemQuality = tab.itemBtn.transform.Find("Quality").GetComponent<UISprite>();
            tab.itemNum = tab.itemBtn.transform.Find("Num").GetComponent<UILabel>();
            tab.itemName = tab.itemBtn.transform.Find("Name").GetComponent<UILabel>();
            tab.btnBuy = item.transform.Find("Btn_Buy").gameObject;
            tab.costMoneyTexture = item.transform.Find("Price/Texture").GetComponent<UITexture>();
            tab.costMoneyNum = item.transform.Find("Price/Num").GetComponent<UILabel>();
            mStoreList.Add(tab);
        }
    }

    private void CreateBottomItems()
    {
        Dictionary<int, DStoreType>.Enumerator em = ReadCfgStoreType.Dict.GetEnumerator();
        int group = GTWindowManager.Instance.GetToggleGroupId();
        while(em.MoveNext())
        {
            GameObject item = NGUITools.AddChild(bottomGrid.gameObject, bottomTemp);
            item.SetActive(true);

            UISprite storeIcon1 = item.transform.GetComponent<UISprite>();
            UISprite storeIcon2 = item.transform.Find("Icon").GetComponent<UISprite>();
            UILabel  storeName = item.transform.Find("Name").GetComponent<UILabel>();
            GameObject storeBtn = item;
            UIToggle storeToggle = item.GetComponent<UIToggle>();
            storeToggle.group = group;
            storeToggle.value = em.Current.Key == mStoreType;

            storeIcon1.spriteName = em.Current.Value.Icon1;
            storeIcon2.spriteName = em.Current.Value.Icon2;
            storeName.text = em.Current.Value.Name;

            int key = em.Current.Key;
            UIEventListener.Get(storeBtn).onClick = delegate (GameObject go)
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                if (mStoreType == key)
                {
                    return;
                }
                mStoreType = key;
                centerView.ResetPosition();
                RefreshView();
            };
        }

        em.Dispose();
    }

    private void RefreshView()
    {
        List<DStore> pList = MLStore.Instance.GetSelectProductListByStoreType(mStoreType);
        for (int i = 0; i < 40; i++)
        {
            StoreItem tab = mStoreList[i];
            if (i >= pList.Count)
            {
                tab.obj.SetActive(false);
                continue;
            }
            tab.obj.SetActive(true);

            DStore db = pList[i];
            DItem itemDB = ReadCfgItem.GetDataById(db.ItemID);
            GTItemHelper.ShowItemTexture(tab.itemTexture, db.ItemID);
            GTItemHelper.ShowItemQuality(tab.itemQuality, db.ItemID);
            GTItemHelper.ShowItemName(tab.itemName, db.ItemID);
            tab.itemNum.text = db.ItemNum.ToString();

            if (db.CostMoneyID > 0)
            {
                tab.costMoneyNum.text = db.CostMoneyNum.ToString();
                GTItemHelper.ShowItemTexture(tab.costMoneyTexture, db.CostMoneyID);
            }
            else
            {
                tab.costMoneyNum.text = string.Empty;
                tab.costMoneyTexture.gameObject.SetActive(false);
            }


            UIEventListener.Get(tab.btnBuy).onClick = delegate (GameObject go)
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                GTNetworkSend.Instance.TryBuyStore(mStoreType, db.Id, 1);
            };

            UIEventListener.Get(tab.itemBtn).onClick = delegate (GameObject go)
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                GTItemHelper.ShowItemDialogById(itemDB.Id);
            };
        }
    }
}
