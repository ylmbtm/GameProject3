using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIBag : GTWindow
{
    public UIBag()
    {
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.Black;
        Path = "Bag/UIBag";
        Resident = false;
        ShowMode = EWindowShowMode.HideOther;
        mLeftWindows[EBagType.ITEM] = EWindowID.UIRoleEquip;
        mLeftWindows[EBagType.CHIP] = EWindowID.UIRoleEquip;
        mLeftWindows[EBagType.TASK] = EWindowID.UIRoleEquip;
        mLeftWindows[EBagType.FASHION] = EWindowID.UIRoleFashion;
        mLeftWindows[EBagType.GEM] = EWindowID.UIRoleGem;
        mLeftWindows[EBagType.RUNE] = EWindowID.UIRoleRune;
        mLeftWindows[EBagType.CARD] = EWindowID.UIRoleFetter;
    }

    protected override void OnAwake()
    {
        btnClose = transform.Find("Pivot/Btn_Close").gameObject;
        btnArrange = transform.Find("Pivot/Btn_Arrange").gameObject;
        btnOneKeyToSell = transform.Find("Pivot/Btn_OneKeyToSell").gameObject;
        labBagNum = transform.Find("Pivot/BagNum").GetComponent<UILabel>();
        labArrange = transform.Find("Pivot/Btn_Arrange/Label").GetComponent<UILabel>();

        mBagMenus.Add(EBagType.ITEM, transform.Find("Tables/Tab_Item").gameObject);
        mBagMenus.Add(EBagType.CHIP, transform.Find("Tables/Tab_Chip").gameObject);
        mBagMenus.Add(EBagType.GEM, transform.Find("Tables/Tab_Gem").gameObject);
        mBagMenus.Add(EBagType.FASHION, transform.Find("Tables/Tab_Fashion").gameObject);
        mBagMenus.Add(EBagType.RUNE, transform.Find("Tables/Tab_Rune").gameObject);
        mBagMenus.Add(EBagType.TASK, transform.Find("Tables/Tab_Task").gameObject);
        mBagMenus.Add(EBagType.CARD, transform.Find("Tables/Tab_Fetter").gameObject);

        bagView = transform.Find("Pivot/Bag/View").GetComponent<UIScrollView>();
        itemPrefab = transform.Find("Pivot/Bag/View/Prefab").gameObject;
        itemGrid = transform.Find("Pivot/Bag/View/Grid").GetComponent<UIGrid>();

        int bagNum = GTDataManager.Instance.GetBagNum();
        itemPrefab.SetActive(false);
        for (int i = 1; i <= bagNum; i++)
        {
            ItemCell item = new ItemCell();
            item.itemBtn = NGUITools.AddChild(itemGrid.gameObject, itemPrefab);
            item.itemBtn.SetActive(true);
            itemGrid.AddChild(item.itemBtn.transform);
            item.itemBtn.name = i.ToString();
            item.itemTexture = item.itemBtn.transform.Find("Texture").GetComponent<UITexture>();
            item.itemQuality = item.itemBtn.transform.Find("Quality").GetComponent<UISprite>();
            item.itemNum = item.itemBtn.transform.Find("Num").GetComponent<UILabel>();
            item.itemChip = item.itemBtn.transform.Find("Chip").gameObject;
            item.itemChip.SetActive(false);
            mBagCells.Add(item);
        }
        SetBagToggleGroup();

    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnOneKeyToSell).onClick = OnOneKeyToSellClick;
        UIEventListener.Get(btnArrange).onClick = OnArrangeClick;

        for (int i = 0; i < mBagCells.Count; i++)
        {
            int index = i;
            UIEventListener.Get(mBagCells[index].itemBtn).onClick = OnBagCellClick;
        }

        foreach(var current in mBagMenus)
        {
            EBagType type = current.Key;
            GameObject menu = current.Value;
            UIEventListener.Get(menu).onClick = (GameObject go) =>
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                if (type == mShow) return;
                ShowBagView(type);
            };
        }
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_BAG_SORT_BAG,               OnRecvSortBag);
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_USE_ITEM,     OnRecvUseItem);
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_DRESS_EQUIP,  OnRecvDressEquip);
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_UNLOAD_EQUIP, OnRecvUnloadEquip);
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_DRESS_GEM,    OnRecvDressGem);
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_UNLOAD_GEM,   OnRecvUnloadGem);
        GTEventCenter.AddHandler<int>(GTEventID.TYPE_BAG_COMPOSE_CHIP,      OnRecvComposeChip);
        GTEventCenter.AddHandler(GTEventID.TYPE_BAG_STRENGTHEN_EQUIP,       OnRecvStrengthEquip);
        GTEventCenter.AddHandler(GTEventID.TYPE_BAG_ADVANCE_EQUIP,          OnRecvAdvanceEquip);
        GTEventCenter.AddHandler(GTEventID.TYPE_BAG_UPSTAR_EQUIP,           OnRecvUpStarEquip);
        GTEventCenter.AddHandler(GTEventID.TYPE_BAG_STRENGTHEN_GEM,         OnRecvStrengthGem);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_BAG_SORT_BAG,               OnRecvSortBag);
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_DRESS_EQUIP,  OnRecvDressEquip);
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_UNLOAD_EQUIP, OnRecvUnloadEquip);
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_DRESS_GEM,    OnRecvDressGem);
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_UNLOAD_GEM,   OnRecvUnloadGem);
        GTEventCenter.DelHandler<int>(GTEventID.TYPE_BAG_COMPOSE_CHIP,      OnRecvComposeChip);
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_USE_ITEM,     OnRecvUseItem);
        GTEventCenter.DelHandler(GTEventID.TYPE_BAG_STRENGTHEN_EQUIP,       OnRecvStrengthEquip);
        GTEventCenter.DelHandler(GTEventID.TYPE_BAG_ADVANCE_EQUIP,          OnRecvAdvanceEquip);
        GTEventCenter.DelHandler(GTEventID.TYPE_BAG_UPSTAR_EQUIP,           OnRecvUpStarEquip);
        GTEventCenter.DelHandler(GTEventID.TYPE_BAG_STRENGTHEN_GEM,         OnRecvStrengthGem);

    }

    protected override void OnEnable()
    {
        GTCoroutinueManager.Instance.StartCoroutine(OpenFirstView());
    }

    protected override void OnClose()
    {
        mBagCells.Clear();
        mBagMenus.Clear();
        mShow = EBagType.NONE;
    }

    IEnumerator  OpenFirstView()
    {
        yield return null;
        ShowBagView(EBagType.ITEM);
    }

    private void SetBagToggleGroup()
    {
        int group = GTWindowManager.Instance.GetToggleGroupId();
        foreach (var current in mBagMenus)
        {
            current.Value.GetComponent<UIToggle>().group = group;
        }
    }

    private void OnBagCellClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        int pos = go.name.ToInt32();
        GTItemHelper.ShowItemDialogByBagType(mShow, pos);
    }

    private void OnArrangeClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (mRemainTimer > 0)
        {
            return;
        }
        GTNetworkSend.Instance.TrySortBag(mShow);
        mRemainTimer = 20;
        GTTimerManager.Instance.AddListener(1, SortClock, 20);
        ShowSortRemainTimer();
    }

    private void OnOneKeyToSellClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        if (mLeftWindows.ContainsKey(mShow))
        {
            EWindowID closeID = mLeftWindows[mShow];
            GTWindowManager.Instance.HideWindow(closeID);
        }
        Hide();
    }

    private void SortClock()
    {
        if (mRemainTimer > 0) mRemainTimer--;
        if (transform == null) return;
        ShowSortRemainTimer();
    }

    private void ShowSortRemainTimer()
    {
        if (mRemainTimer > 0)
        {
            labArrange.text = GTTools.Format("整理({0})", mRemainTimer);
        }
        else
        {
            labArrange.text = "整理";
        }
    }

    private void OnRecvUnloadEquip(int arg1, int arg2)
    {
        ShowCellView(arg1 - 1);
        ShowBagNum();
    }

    private void OnRecvDressEquip(int arg1, int arg2)
    {
        ShowCellView(arg1 - 1);
        ShowBagNum();
    }

    private void OnRecvSortBag()
    {
        RefreshBagView();
    }

    private void OnRecvUnloadGem(int newPos, int targetPos)
    {
        ShowCellView(newPos - 1);
        ShowBagNum();
    }

    private void OnRecvDressGem(int sourcePos, int targetPos)
    {
        ShowCellView(sourcePos - 1);
        ShowBagNum();
    }

    private void ShowBagView(EBagType bagType)
    {
        LoadLeftWindow(bagType);
        bagView.ResetPosition();
        RefreshBagView();
    }

    private void OnRecvComposeChip(int arg1)
    {
        ShowCellView(arg1 - 1);
        ShowBagNum();
    }

    private void OnRecvUseItem(int id, int num)
    {
        RefreshBagView();
        ShowBagNum();
    }

    private void OnRecvStrengthGem()
    {
        RefreshBagView();
        ShowBagNum();
    }

    private void OnRecvUpStarEquip()
    {
        RefreshBagView();
        ShowBagNum();
    }

    private void OnRecvAdvanceEquip()
    {
        RefreshBagView();
        ShowBagNum();
    }

    private void OnRecvStrengthEquip()
    {
        RefreshBagView();
        ShowBagNum();
    }

    private void RefreshBagView()
    {
        for (int i = 0; i < mBagCells.Count; i++)
        {
            ShowCellView(i);
        }
        ShowBagNum();
    }

    private void ShowBagNum()
    {
        int maxNum = GTDataManager.Instance.GetBagNum();
        int curNum = GTDataManager.Instance.GetBagDataByBagType(mShow).Count;
        labBagNum.text = GTTools.Format("{0}/{1}", curNum, maxNum);
    }

    private void LoadLeftWindow(EBagType bagType)
    {
        if (bagType == mShow)
        {
            return;
        }
        if (mLeftWindows.ContainsKey(mShow))
        {
            EWindowID closeWindowID = mLeftWindows[mShow];
            GTWindowManager.Instance.HideWindow(closeWindowID);
        }
        if (mLeftWindows.ContainsKey(bagType))
        {
            EWindowID openWindowID = mLeftWindows[bagType];
            GTWindowManager.Instance.OpenWindow(openWindowID);
        }
        mShow = bagType;
    }

    private void ShowCellView(int index)
    {
        Dictionary<int, XItem> bagData = GTDataManager.Instance.GetBagDataByBagType(mShow);
        ItemCell cell = mBagCells[index];
        int pos = index + 1;
        XItem item = null;
        if(bagData.ContainsKey(pos))
        {
            item = bagData[pos];
        }
        if(item==null)
        {
            cell.Show(false);
            return;
        }
        cell.Show(true);
        cell.itemNum.text = item.Num > 1 ? item.Num.ToString() : string.Empty;
        GTItemHelper.ShowItemTexture(cell.itemTexture, item.Id);
        GTItemHelper.ShowItemQuality(cell.itemQuality, item.Id);
        cell.itemChip.SetActive(mShow == EBagType.CHIP);
    }

    private GameObject                       btnClose;
    private GameObject                       btnOneKeyToSell;
    private GameObject                       btnArrange;

    private UILabel                          labArrange;
    private UILabel                          labBagNum;
    private GameObject                       itemPrefab;
    private UIGrid                           itemGrid;
    private UIScrollView                     bagView;

    private List<ItemCell>                   mBagCells     = new List<ItemCell>();
    private int                              mRemainTimer  = 0;
    private EBagType                         mShow         = EBagType.NONE;
    private Dictionary<EBagType, EWindowID>  mLeftWindows  = new Dictionary<EBagType, EWindowID>();
    private Dictionary<EBagType, GameObject> mBagMenus     = new Dictionary<EBagType, GameObject>();

    class ItemCell
    {
        public UITexture  itemTexture;
        public UISprite   itemQuality;
        public UILabel    itemNum;
        public GameObject itemBtn;
        public GameObject itemChip;

        public void Show(bool isHasEmpty, bool isNew = false)
        {
            itemTexture.gameObject.SetActive(isHasEmpty);
            itemQuality.gameObject.SetActive(isHasEmpty);
            itemNum.gameObject.SetActive(isHasEmpty);
            itemChip.SetActive(isHasEmpty);
        }
    }
}
