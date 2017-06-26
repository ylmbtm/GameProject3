using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class UIMainRaid : GTWindow
{
    public UIMainRaid()
    {
        mResident = false;
        mResPath = "Raid/UIMainRaid";
        Type = EWindowType.WINDOW;
    }

    protected override void OnAwake()
    {
        for(int i=1;i<=3;i++)
        {
            GameObject btn = transform.Find("Top/Boxs/" + i.ToString()).gameObject;
            mBoxBtns[i-1] = btn;
            mBoxStates[i-1] = btn.GetComponent<UISprite>();
            mBoxStarNums[i-1] = btn.transform.Find("StarNum").GetComponent<UILabel>();
        }

        btnReturn = transform.Find("Top/Btn_Return").gameObject;
        mMapTexture = transform.Find("MapTexture").GetComponent<UITexture>();
        mChapterName = transform.Find("Top/ChapterName").GetComponent<UILabel>();
        mCopysParent = transform.Find("Copys");
        mLight = mCopysParent.Find("Light").gameObject;

        for(int i=1;i<=6;i++)
        {
            ItemCopy item = new ItemCopy();
            GameObject go = mCopysParent.Find(i.ToString()).gameObject;
            item.item = go;
            for (int k = 1; k <= 3; k++)
            {
                item.stars[k - 1] = go.transform.Find("Star/Star" + k.ToString()).GetComponent<UISprite>();
            }
            item.icon = go.transform.Find("Icon").GetComponent<UISprite>();
            item.copyName = go.transform.Find("CopyName").GetComponent<UILabel>();
            mCopyItems.Add(item);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnReturn).onClick = OnReturnClick;
        for(int i=0;i<3;i++)
        {
            UIEventListener.Get(mBoxBtns[i]).onClick = OnBoxClick;
        }
        for(int i=0;i<mCopyItems.Count;i++)
        {
            UIEventListener.Get(mCopyItems[i].item).onClick = OnCopyClick;
        }

    }

    private void OnCopyClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        DWorld chapterDB = ReadCfgWorld.GetDataById(mChapter);
        int enterIndex = RaidModule.Instance.GetMaxCanEnterCopyIndex(mCurCopyType, mChapter);
        int index = go.name.ToInt32();
        if(index>enterIndex)
        {
            GTItemHelper.ShowTip("未通关前置关卡");
            return;
        }
        GTWindowManager.Instance.OpenWindow(EWindowID.UI_MAINGATE);
        UIMainGate window = (UIMainGate)GTWindowManager.Instance.GetWindow(EWindowID.UI_MAINGATE);
        window.ShowViewById(mCurCopyType, mChapter, chapterDB.Copys[index-1]);
    }


    private void OnBoxClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        int index = go.name.ToInt32() - 1;
        DWorld db = ReadCfgWorld.GetDataById(mChapter);
        GTWindowManager.Instance.OpenWindow(EWindowID.UI_AWARDBOX);
        UIAwardBox window = (UIAwardBox)GTWindowManager.Instance.GetWindow(EWindowID.UI_AWARDBOX);
        int state = RaidModule.Instance.GetChapterRewardStateByAwardIndex(mChapter, index);
        UIEventListener.VoidDelegate doReceive = delegate (GameObject w)
        {
            RaidService.Instance.TryReceiveReward(mCurCopyType, mChapter, index);
        };
        window.ShowAwardById(db.Awards[index], state, doReceive);
    }

    private void OnReturnClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    protected override void OnAddHandler()
    {
        
    }

    protected override void OnEnable()
    {
        GuideManager.Instance.AddListener("Btn_Copy1", mCopyItems[0].item.transform);
    }

    protected override void OnDelHandler()
    {
        
    }

    protected override void OnClose()
    {
        mCopyItems.Clear();
        mCurCopyType = ECopyType.TYPE_EASY;
        GuideManager.Instance.DelListener("Btn_Copy1");
    }


    public void ShowViewByChapter(int chapterID)
    {
        mChapter = chapterID;
        DWorld chapterDB = ReadCfgWorld.GetDataById(chapterID);

        for (int i = 0; i < 3; i++)
        {
            mBoxStarNums[i].text = chapterDB.Stars[i].ToString();
            int rewardState = RaidModule.Instance.GetChapterRewardStateByAwardIndex(chapterID, i);
            switch(rewardState)
            {
                case ERewardState.NOT_RECEIVE:
                case ERewardState.CAN_RECEIVE:
                    mBoxStates[i].spriteName = "613";
                    break;
                case ERewardState.HAS_RECEIVE:
                    mBoxStates[i].spriteName = "614";
                    break;
            }
        }

        int copysNum = chapterDB.Copys.Length;
        mChapterName.text = chapterDB.Name;
        GTItemHelper.ShowTexture(mMapTexture, chapterDB.Map);
        int enterIndex = RaidModule.Instance.GetMaxCanEnterCopyIndex(mCurCopyType, chapterID);
        for(int i=0;i<mCopyItems.Count;i++)
        {
            ItemCopy item = mCopyItems[i];
            if (i<copysNum)
            {
                item.item.SetActive(true);
                item.Show(i < enterIndex);                
                int copyID = chapterDB.Copys[i];
                DCopy db = ReadCfgCopy.GetDataById(copyID);
                int starNum = RaidModule.Instance.GetCopyStarNumById(copyID);
                item.copyName.text = db.Name;
                item.item.transform.localPosition = chapterDB.CopyPosArray[i];
                GTItemHelper.ShowStarNum(item.stars, starNum);
            }
            else
            {
                mCopyItems[i].item.SetActive(false);
            }
            if(i==enterIndex-1)
            {
                mLight.gameObject.SetActive(false);
                mLight.transform.parent = item.item.transform;
                mLight.transform.localPosition =Vector3.zero;
            }
        }
    }


    private GameObject[] mBoxBtns = new GameObject[3];
    private UISprite[] mBoxStates = new UISprite[3];
    private UILabel[] mBoxStarNums = new UILabel[3];

    private UITexture mMapTexture;
    private GameObject btnReturn;
    private UILabel mChapterName;
    private Transform mCopysParent;
    private GameObject mLight;
    private List<ItemCopy> mCopyItems = new List<ItemCopy>();
    private ECopyType mCurCopyType = ECopyType.TYPE_EASY;
    private int mChapter = 0;

    class ItemCopy
    {
        public UISprite icon;
        public UISprite[] stars = new UISprite[3];
        public UILabel copyName;
        public GameObject item;

        public void Show(bool isPass)
        {

            Color color = isPass ? Color.white : Color.black;
            icon.color = color;
            for(int i=0;i<stars.Length;i++)
            {
                stars[i].color = color;
            }
        }
    }
}
