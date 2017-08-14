using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using MAP;
using Protocol;

public class UIMainResult : GTWindow
{
    private Transform         mVictory;
    private Transform         mFailture;
    private GameObject        mVictoryAward;
    private ItemMoney[]       mMoneys;
    private UIGrid            mGrid;
    private GameObject        mTemp;
    private Transform[]       mNewStars;
    private GameObject        mContinue;
    private bool              mCanClickMask = false;
    private Transform         mMoneysTrans;
    private int               mStarIndex = 0;
    private int               mItemIndex = 0;

    private GameObject        mFailtureFont;
    private GameObject        btnMask;
    private GameObject        btnQuit;
    private GameObject        btnRecover;
    private GameObject        mRecover;
    private UITexture         mRecoverCostTexture;
    private UILabel           mRecoverCostNum;
    private ItemPassContent[] mPassContents;

    class ItemMoney
    {
        public GameObject     money;
        public UITexture      moneyTexture;
        public UILabel        moneyNum;
    }

    class ItemPassContent
    {
        public Transform      trans;
        public UISprite       icon;
        public UILabel        text;
    }

    public UIMainResult()
    {
        Path = "Raid/UIMainResult";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.BlackTransparent;
        ShowMode = EWindowShowMode.DoNothing;
        Resident = false;
    }

    protected override void OnAwake()
    {
        mStarIndex = 0;
        mItemIndex = 0;
        mCanClickMask = false;

        Transform pivot = transform.Find("Pivot");
        mVictory = pivot.Find("Victory").transform;
        mFailture = pivot.Find("Failture").transform;
        mVictoryAward = mVictory.transform.Find("Award").gameObject;
        mMoneysTrans = mVictory.transform.Find("Moneys");

        mMoneys = new ItemMoney[2];
        for (int i = 1; i <= 2; i++)
        {
            Transform m = mVictory.transform.Find("Moneys/" + i);
            ItemMoney tab = new ItemMoney();
            tab.moneyTexture = m.Find("Texture").GetComponent<UITexture>();
            tab.money = m.gameObject;
            tab.moneyNum = m.Find("Num").GetComponent<UILabel>();
            mMoneys[i - 1] = tab;
        }
        mGrid = mVictory.transform.Find("Grid").GetComponent<UIGrid>();
        mTemp = mVictory.transform.Find("Temp").gameObject;


        mNewStars = new Transform[3];
        for (int i = 1; i <= 3; i++)
        {
            Transform trans = mVictory.transform.Find("NewStars/" + i);
            trans.gameObject.SetActive(false);
            mNewStars[i - 1] = trans;
        }
        mContinue = mVictory.transform.Find("Bottom/Continue").gameObject;
        mFailtureFont = mFailture.transform.Find("FailtureFont").gameObject;

        btnQuit   = mFailture.transform.Find("Btn_Quit").gameObject;
        btnRecover= mFailture.transform.Find("Btn_Recover").gameObject;
        mRecover = mFailture.transform.Find("Recover").gameObject;
        mRecoverCostTexture = mRecover.transform.Find("Texture").GetComponent<UITexture>();
        mRecoverCostNum= mRecover.transform.Find("Num").GetComponent<UILabel>();
        mPassContents = new ItemPassContent[3];
        for (int i = 1; i <= 3; i++)
        {
            Transform trans = mFailture.transform.Find("FailtureContent/" + i);
            trans.gameObject.SetActive(false);
            ItemPassContent tab = new ItemPassContent();
            tab.trans = trans;
            tab.icon = trans.Find("Icon").GetComponent<UISprite>();
            tab.text = trans.Find("Text").GetComponent<UILabel>();
            mPassContents[i - 1] = tab;
        }
        btnMask = transform.Find("Mask").gameObject;
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnMask).onClick = OnBtnMaskClick;
        UIEventListener.Get(btnQuit).onClick = OnQuitClick;
        UIEventListener.Get(btnRecover).onClick = OnRecoverClick;
    }

    protected override void OnAddHandler()
    {

    }

    protected override void OnEnable()
    {
        InitView();
        ShowView();
    }

    protected override void OnDelHandler()
    {

    }

    protected override void OnClose()
    {

    }

    private void OnRecoverClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTItemHelper.ShowTip("功能未开放");
    }

    private void OnQuitClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTLauncher.Instance.LoadScene(GTGlobal.LastCityID);
    }

    private void OnBtnMaskClick(GameObject go)
    {
        if (mCanClickMask == false)
        {
            return;
        }
        GTLauncher.Instance.LoadScene(GTGlobal.LastCityID);
    }

    private void ShowView()
    {
        if(FTEctypeData.Win)
        {
            ShowWinView();
            GTAudioManager.Instance.PlaySound(GTAudioKey.SOUND_BATTLE_WIN);
        }
        else
        {
            ShowFailView();
            GTAudioManager.Instance.PlaySound(GTAudioKey.SOUND_BATTLE_FAIL);
        }
    }

    private void InitView()
    {
        mVictory.gameObject.SetActive(false);
        mMoneysTrans.gameObject.SetActive(false);
        mVictoryAward.SetActive(false);
        mGrid.gameObject.SetActive(false);
        mTemp.SetActive(false);
        mContinue.SetActive(false);

        mFailture.gameObject.SetActive(false);
        mFailtureFont.SetActive(false);
        btnQuit.SetActive(false);
        btnRecover.SetActive(false);
        mRecover.SetActive(false);
    }

    private void ShowWinView()
    {
        mVictory.gameObject.SetActive(true);

        float sec = 0;
        for (int i = 0; i < FTEctypeData.Star; i++)
        {
            sec = 0.4f * (i + 1);
            GTTimerManager.Instance.AddListener(sec, PlayWinStarTween);
        }
        sec += 0.3f;
        GTTimerManager.Instance.AddListener(sec, PlayWinAwardTween);
        sec += 0.3f;
        GTTimerManager.Instance.AddListener(sec, PlayWinMoneyTween);
        InitWinItems();
        for (int i = 0; i < mGrid.transform.childCount; i++)
        {
            sec += 0.1f;
            GTTimerManager.Instance.AddListener(sec, PlayWinItemTween);
        }
        sec += 0.1f;
        GTTimerManager.Instance.AddListener(sec, PlayWinContinueTween);
    }

    private void PlayWinStarTween()
    {
        TweenScale[] tsArray = mNewStars[mStarIndex].GetComponents<TweenScale>();
        tsArray[0].delay = 0.0f;
        tsArray[0].duration = 0.2f;
        tsArray[0].from = new Vector3(5f, 5f, 5f);
        tsArray[0].to   = new Vector3(0.75f,0.75f, 0.75f);
        tsArray[0].ResetToBeginning();
        tsArray[0].PlayForward();

        tsArray[1].delay = 0.2f;
        tsArray[1].duration = 0.2f;
        tsArray[1].from = new Vector3(0.75f, 0.75f, 0.75f);
        tsArray[1].to = new Vector3(1f, 1f, 1f);
        tsArray[1].ResetToBeginning();
        tsArray[1].PlayForward();

        mNewStars[mStarIndex].gameObject.SetActive(true);
        mStarIndex++;
    }

    private void PlayWinAwardTween()
    {
        mVictoryAward.SetActive(true);
    }

    private void PlayWinMoneyTween()
    {
        mMoneysTrans.gameObject.SetActive(true);
        DCopy db = ReadCfgCopy.GetDataById(FTEctypeData.CopyID);
        if (db == null)
        {
            return;
        }
        XCharacter role = GTGlobal.Main;
        int[] idArray = new int[2] { db.GetMoneyId, 50 };
        int[] numArray = new int[2] { db.GetMoneyRatio * role.Level, db.GetExpRatio * role.Level };
        for (int i = 0; i < mMoneys.Length; i++)
        {
            ItemMoney tab = mMoneys[i];
            GTItemHelper.ShowItemTexture(tab.moneyTexture, idArray[i]);
            GTItemHelper.ShowItemNum(tab.moneyNum, numArray[i]);
        }
    }

    private void InitWinItems()
    {
        mGrid.gameObject.SetActive(true);
        DCopy db = ReadCfgCopy.GetDataById(FTEctypeData.CopyID);
        if (db == null)
        {
            return;
        }
        List<GTItem> list = GTAwardHelper.GetAwardDataByID(db.AwardId);
        if (list == null)
        {
            return;
        }
        for (int i = 0; i < list.Count; i++)
        {
            GameObject item = NGUITools.AddChild(mGrid.gameObject, mTemp);
            mGrid.AddChild(item.transform);

            UITexture itemTexture = item.transform.Find("Texture").GetComponent<UITexture>();
            UISprite itemQuality = item.transform.Find("Quality").GetComponent<UISprite>();
            UILabel itemNum = item.transform.Find("Num").GetComponent<UILabel>();
            GameObject itemChip = item.transform.Find("Chip").gameObject;
            GTItem data = list[i];
            GTItemHelper.ShowItemTexture(itemTexture, data.Id);
            GTItemHelper.ShowItemQuality(itemQuality, data.Id);
            GTItemHelper.ShowItemNum(itemNum, data.Num);
            GTItemHelper.ShowItemChip(itemChip, data.Id);
            UIEventListener.Get(item).onClick = delegate (GameObject go)
            {
                GTItemHelper.ShowItemDialogById(data.Id);
            };
        }
        mGrid.repositionNow = true;
    }

    private void PlayWinItemTween()
    {
        mGrid.GetChild(mItemIndex).gameObject.SetActive(true);
        mItemIndex++;
    }

    private void PlayWinContinueTween()
    {
        mContinue.SetActive(true);
        mContinue.GetComponent<UILabel>().text = "点击屏幕继续";
        mCanClickMask = true;
    }

    private void ShowFailView()
    {
        mFailture.gameObject.SetActive(true);  
        float sec = 0.3f;
        GTTimerManager.Instance.AddListener(sec, PlayFailFontTween);
        sec += 0.3f;
        GTTimerManager.Instance.AddListener(sec, PlayFailContentTween);
        sec += 0.3f;
        GTTimerManager.Instance.AddListener(sec, PlayFailBottomTween);
    }

    private void PlayFailFontTween()
    {
        TweenScale[] tsArray = mFailtureFont.GetComponents<TweenScale>();
        for (int i = 0; i < tsArray.Length; i++)
        {
            tsArray[i].ResetToBeginning();
            tsArray[i].PlayForward();
        }
        mFailtureFont.SetActive(true);
    }

    private void PlayFailContentTween()
    {
        DCopy db = ReadCfgCopy.GetDataById(FTEctypeData.CopyID);
        if (db == null)
        {
            return;
        }
        for (int i = 0; i < mPassContents.Length; i++)
        {
            ItemPassContent tab = mPassContents[i];
            EStarCondition type = db.StarConditions[i];
            int v = db.StarValues[i];
            switch (type)
            {
                case EStarCondition.TYPE_MAIN_HEALTH:
                    tab.text.text = GTTools.Format("主角血量大于{0}%", v);
                    break;
                case EStarCondition.TYPE_PASSCOPY:
                    tab.text.text = "通关副本";
                    break;
                case EStarCondition.TYPE_TIME_LIMIT:
                    tab.text.text = GTTools.Format("{0}秒通关副本", v);
                    break;
            }
            tab.trans.gameObject.SetActive(true);
        }
    }

    private void PlayFailBottomTween()
    {
        mRecover.SetActive(true);
        btnQuit.SetActive(true);
        btnRecover.SetActive(true);
        mRecoverCostNum.text = GTDefine.RECOVER_COST_ITEM_NUM.ToString();
        GTItemHelper.ShowItemTexture(mRecoverCostTexture, GTDefine.RECOVER_COST_ITEM_ID);
    }
}
