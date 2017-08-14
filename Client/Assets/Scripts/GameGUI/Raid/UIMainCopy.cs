using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class UIMainCopy : GTWindow
{
    private GameObject      btnEnter;
    private GameObject      btnSweep;
    private GameObject      btnOneKeyToSweep;
    private GameObject      btnClose;
    private UITexture       costActionTexture;
    private UILabel         costActionNum;
    private UITexture       gateTexture;
    private UILabel         gateName;
    private UILabel         gateCopyDiffculty;
    private UILabel         gateDesc;
    private UILabel         gateFightValue;
    private UILabel         gateBattleTimes;
    private List<ItemAward> items = new List<ItemAward>();

    private int             mCopyDiffculty;
    private int             mChapter;
    private int             mCopyID;

    class ItemAward
    {
        public UITexture    itemTexture;
        public UISprite     itemQuality;
        public GameObject   itemBtn;
        public GameObject   itemChip;
    }

    public UIMainCopy()
    {
        Path = "Raid/UIMainCopy";
        Resident = false;
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
        ShowMode = EWindowShowMode.SaveTarget;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnEnter = pivot.Find("BtnEnter").gameObject;
        btnClose = pivot.Find("BtnClose").gameObject;
        btnSweep = pivot.Find("BtnSweep").gameObject;
        btnOneKeyToSweep = pivot.Find("BtnOneKeyToSweep").gameObject;
        costActionTexture = pivot.Find("CopyCostAction/Icon").GetComponent<UITexture>();
        costActionNum = pivot.Find("CopyCostAction/Num").GetComponent<UILabel>();
        gateTexture = pivot.Find("CopyInfo/CopyTexture").GetComponent<UITexture>();
        gateDesc = pivot.Find("CopyInfo/CopyDesc").GetComponent<UILabel>();
        gateFightValue = pivot.Find("CopyInfo/CopyRequireFightValue").GetComponent<UILabel>();
        gateCopyDiffculty = pivot.Find("CopyInfo/CopyDifficulty").GetComponent<UILabel>();
        gateBattleTimes = pivot.Find("CopyInfo/CopyBattleTimes").GetComponent<UILabel>();
        gateName = pivot.Find("CopyName").GetComponent<UILabel>();

        for (int i = 1; i <= 5; i++)
        {
            ItemAward it = new ItemAward();
            it.itemBtn = pivot.Find("CopyItems/" + i).gameObject;
            it.itemTexture = it.itemBtn.transform.Find("Texture").GetComponent<UITexture>();
            it.itemQuality = it.itemBtn.transform.Find("Quality").GetComponent<UISprite>();
            it.itemChip = it.itemBtn.transform.Find("Chip").gameObject;
            items.Add(it);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(btnEnter).onClick = OnEnterClick;
        UIEventListener.Get(btnSweep).onClick = OnSweepClick;
        UIEventListener.Get(btnOneKeyToSweep).onClick = OnOneKeyToSweepClick;
    }

    protected override void OnAddHandler()
    {

    }

    protected override void OnEnable()
    {

    }

    protected override void OnDelHandler()
    {

    }

    protected override void OnClose()
    {
        items.Clear();
    }

    private void OnOneKeyToSweepClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnSweepClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnEnterClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryMainCopyReq(GTGlobal.CurPlayerGUID, mCopyID);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void ShowView()
    {
        DCopy copyDB = ReadCfgCopy.GetDataById(mCopyID);
        gateCopyDiffculty.text = "简单模式";
        GTItemHelper.ShowTexture(gateTexture, copyDB.Texture);
        gateName.text = copyDB.Name;
        gateDesc.text = copyDB.Desc;
        gateFightValue.text = GTTools.Format("推荐战力值：{0}", copyDB.FightValue);
        gateBattleTimes.text = string.Empty;
        costActionNum.text = copyDB.CostActionNum.ToString();
        GTItemHelper.ShowItemTexture(costActionTexture,copyDB.CostActionId);
        ShowAwardItemsView();
    }

    private void ShowAwardItemsView()
    {
        DCopy copyDB = ReadCfgCopy.GetDataById(mCopyID);
        int awardID = copyDB.AwardId;
        List<GTItem> list = GTAwardHelper.GetAwardDataByID(awardID);
        if (list == null)
        {
            return;
        }
        for (int i = 0; i < items.Count; i++)
        {
            ItemAward tab = items[i];
            if (i < items.Count)
            {
                GTItem data = list[i];
                tab.itemBtn.SetActive(true);
                GTItemHelper.ShowItemTexture(tab.itemTexture, data.Id);
                GTItemHelper.ShowItemQuality(tab.itemQuality, data.Id);
                GTItemHelper.ShowItemChip(tab.itemChip, data.Id);
                UIEventListener.Get(tab.itemBtn).onClick = delegate (GameObject go)
                {
                    GTItemHelper.ShowItemDialogById(data.Id);
                };
            }
            else
            {
                tab.itemBtn.SetActive(false);
            }
        }
    }

    public void  ShowViewById(int copyDiffculty, int chapter, int copyID)
    {
        mCopyDiffculty = copyDiffculty;
        mChapter       = chapter;
        mCopyID        = copyID;
        ShowView();
    }
}
