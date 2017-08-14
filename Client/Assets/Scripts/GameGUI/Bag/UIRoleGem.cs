using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIRoleGem : GTWindow
{ 
    public UIRoleGem()
    {
        MaskType = EWindowMaskType.None;
        ShowMode = EWindowShowMode.SaveTarget;
        Type = EWindowType.Window;
        Path = "Bag/UIRoleGem";
        Resident = false;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        equipGrid = pivot.Find("Equips/Grid").GetComponent<UIGrid>();
        labPropertys = pivot.Find("Propertys/Text").GetComponent<UILabel>();
        btnOneKeyToDress = pivot.Find("Btn_OneKeyToDress").gameObject;
        btnOneKeyToUnload = pivot.Find("Btn_OneKeyToUnload").gameObject;
        for (int i = 1; i <= 8; i++)
        {
            ItemEquip item = new ItemEquip();
            item.toggle = equipGrid.transform.Find(i.ToString()).GetComponent<UIToggle>();
            item.itemTexture = item.toggle.transform.Find("Texture").GetComponent<UITexture>();
            item.itemQuality = item.toggle.transform.Find("Quality").GetComponent<UISprite>();
            mEItems.Add(item);
        }

        for (int i = 1; i <= 5; i++)
        {
            ItemGem item = new ItemGem();
            item.itemBtn = pivot.Find("Gems/" + i.ToString()).gameObject;
            item.itemTexture = item.itemBtn.transform.Find("Texture").GetComponent<UITexture>();
            item.itemQuality = item.itemBtn.transform.Find("Quality").GetComponent<UISprite>();
            mGItems.Add(item);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnOneKeyToDress).onClick = OnOneKeyToDressClick;
        UIEventListener.Get(btnOneKeyToUnload).onClick = OneKeyToUnloadClick;

        for (int i = 0; i < mEItems.Count; i++)
        {
            UIEventListener.Get(mEItems[i].toggle.gameObject).onClick = OnEquipItemClick;
        }

        for (int i = 0; i < mGItems.Count; i++)
        {
            UIEventListener.Get(mGItems[i].itemBtn).onClick = OnGemItemClick;
        }
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_DRESS_GEM, OnRecvDressGem);
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_BAG_UNLOAD_GEM, OnRecvUnloadGem);
    }

    protected override void OnEnable()
    {
        ShowEquipsView();
        ShowGemCellsView();
        ShowGemPropertyView();
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_DRESS_GEM, OnRecvDressGem);
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_BAG_UNLOAD_GEM, OnRecvUnloadGem);
    }

    protected override void OnClose()
    {
        mEItems.Clear();
        mGItems.Clear();
    }

    private void OnGemItemClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        int targetPos = go.name.ToInt32();
        int pos = MLGem.Instance.GetDressGemPosByIndexAndTargetPos(mCurIndex, targetPos);
        GTItemHelper.ShowItemDialogByPosType(EPosType.RoleGem,pos);
    }

    private void OnEquipItemClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        mCurIndex = go.name.ToInt32();
        ShowGemCellsView();
        ShowGemPropertyView();
    }

    private void OneKeyToUnloadClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryOneKeyToUnloadGem(mCurIndex);
    }

    private void OnOneKeyToDressClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryOneKeyToDressGem(mCurIndex);
    }

    private void ShowEquipsView()
    {
        int group = GTWindowManager.Instance.GetToggleGroupId();
        for(int i=0;i<mEItems.Count;i++)
        {
            ItemEquip item = mEItems[i];
            item.toggle.group = group;
            XEquip equip= GTDataManager.Instance.GetEquipDataByPos(EPosType.RoleEquip,i + 1);
            if(equip==null)
            {
                item.Show(false);
            }
            else
            {
                item.Show(true);
                GTItemHelper.ShowItemTexture(item.itemTexture, equip.Id);
                GTItemHelper.ShowItemQuality(item.itemQuality, equip.Id);
            }

        }
        mEItems[mCurIndex-1].toggle.value = true;
    }

    private void ShowGemCellView(int pos)
    {
        int gemPos = MLGem.Instance.GetDressGemPosByIndexAndTargetPos(mCurIndex, pos);
        ItemGem cell = mGItems[pos - 1];
        XGem gem = GTDataManager.Instance.GetGemDataByPos(EPosType.RoleGem,gemPos);
        if (gem == null)
        {
            cell.Show(false);
        }
        else
        {
            cell.Show(true);
            GTItemHelper.ShowItemTexture(cell.itemTexture, gem.Id);
            GTItemHelper.ShowItemQuality(cell.itemQuality, gem.Id);
        }

    }

    private void ShowGemCellsView()
    {
        for(int i=0;i< mGItems.Count;i++)
        {
            ShowGemCellView(i + 1);
        }
    }

    private void ShowGemPropertyView()
    {
        labPropertys.text = string.Empty;
        Dictionary<EAttr, int> propertys = GTAttrHelper.GetAllGemPropertysByIndex(mCurIndex);
        GTItemHelper.ShowPropertyText(labPropertys, propertys, false);
    }

    private void OnRecvUnloadGem(int newPos, int targetPos)
    {
        ShowGemCellView(targetPos);
        ShowGemPropertyView();
    }

    private void OnRecvDressGem(int sourcePos, int targetPos)
    {
        ShowGemCellView(targetPos);
        ShowGemPropertyView();
    }

    public int GetCurIndex()
    {
        return mCurIndex;
    }

    private UIGrid          equipGrid;
    private UILabel         labPropertys;
    private GameObject      btnOneKeyToDress;
    private GameObject      btnOneKeyToUnload;
    private int             mCurIndex = 1;
    private List<ItemEquip> mEItems = new List<ItemEquip>();
    private List<ItemGem>   mGItems = new List<ItemGem>();

    class ItemEquip
    {
        public UIToggle     toggle;
        public UITexture    itemTexture;
        public UISprite     itemQuality;

        public void Show(bool hasDress)
        {
            itemTexture.gameObject.SetActive(hasDress);
            itemQuality.gameObject.SetActive(hasDress);
        }
    }

    class ItemGem
    {
        public GameObject   itemBtn;
        public UITexture    itemTexture;
        public UISprite     itemQuality;

        public void Show(bool hasDress)
        {
            itemTexture.gameObject.SetActive(hasDress);
            itemQuality.gameObject.SetActive(hasDress);
        }
    }
}
