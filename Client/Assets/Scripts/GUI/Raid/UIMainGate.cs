using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class UIMainGate : GTWindow
{
    public UIMainGate()
    {
        mResPath = "Raid/UIMainGate";
        mResident = false;
        Type = EWindowType.DIALOG;
    }

    protected override void OnAwake()
    {
        btnEnter = transform.Find("Btn_Enter").gameObject;
        btnClose = transform.Find("Btn_Close").gameObject;
        btnSweep = transform.Find("Btn_Sweep").gameObject;
        btnOneKeyToSweep = transform.Find("Btn_OneKeyToSweep").gameObject;
        costActionTexture = transform.Find("CostAction/Texture").GetComponent<UITexture>();
        costActionNum = transform.Find("CostAction/Num").GetComponent<UILabel>();
        gateTexture = transform.Find("Info/Texture").GetComponent<UITexture>();
        gateDesc = transform.Find("Info/Desc").GetComponent<UILabel>();
        gateFightValue = transform.Find("Info/FightValue").GetComponent<UILabel>();
        gateName = transform.Find("Background/TopBar/Label").GetComponent<UILabel>();
        gateCopyType = transform.Find("Info/CopyType").GetComponent<UILabel>();
        gateBattleTimes = transform.Find("Info/BattleTimes").GetComponent<UILabel>();

        for(int i=1;i<=5;i++)
        {
            ItemAward it = new ItemAward();
            it.itemBtn = transform.Find("Items/" + i).gameObject;
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
        RaidService.Instance.TryBattleCheck(mCopyType, mChapter, mCopyID);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    protected override void OnAddHandler()
    {
        
    }

    protected override void OnEnable()
    {
        GuideManager.Instance.AddListener("Btn_EnterCopy", btnEnter.transform);
    }

    protected override void OnDelHandler()
    {
        
    }

    protected override void OnClose()
    {
        items.Clear();
        GuideManager.Instance.DelListener("Btn_EnterCopy");
    }


    public void ShowViewById(ECopyType copyType,int chapter,int copyID)
    {
        mCopyType = copyType;
        mChapter = chapter;
        mCopyID = copyID;
        ShowView();
    }

    private void ShowView()
    {
        DCopy copyDB = ReadCfgCopy.GetDataById(mCopyID);
        gateCopyType.text = "简单模式";
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
        List<KStruct> list = AwardModule.Instance.GetAwardDataByID(awardID);
        if(list==null)
        {
            return;
        }
        for(int i=0;i<items.Count;i++)
        {
            ItemAward tab = items[i];
            if(i<items.Count)
            {
                KStruct data = list[i];
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

    private GameObject btnEnter;
    private GameObject btnSweep;
    private GameObject btnOneKeyToSweep;
    private GameObject btnClose;
    private UITexture costActionTexture;
    private UILabel costActionNum;
    private UITexture gateTexture;
    private UILabel gateName;
    private UILabel gateCopyType;
    private UILabel gateDesc;
    private UILabel gateFightValue;
    private UILabel gateBattleTimes;
    private List<ItemAward> items = new List<ItemAward>();

    private ECopyType mCopyType;
    private int mChapter;
    private int mCopyID;

    class ItemAward
    {
        public UITexture  itemTexture;
        public UISprite   itemQuality;
        public GameObject itemBtn;
        public GameObject itemChip;
    }
}
