using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class UIMainRaid : GTWindow
{
    private GameObject[]   boxBtns        = new GameObject[3];
    private UISprite[]     boxStates      = new UISprite[3];
    private UILabel[]      boxStarNums    = new UILabel[3];

    private GameObject     btnClose;
    private GameObject     btnPrev;
    private GameObject     btnNext;
    private UILabel        chapterName;
    private Transform      copysParent;
    private List<ItemCopy> copyItems = new List<ItemCopy>();
    private int            chapter = 0;
    private int            copyDifficulty = 1;

    class ItemCopy
    {
        public UISprite    icon;
        public UISprite[]  stars = new UISprite[3];
        public UILabel     copyName;
        public GameObject  item;
    }

    public UIMainRaid()
    {
        Resident = false;
        Path = "Raid/UIMainRaid";
        Type = EWindowType.Window;
        MaskType = EWindowMaskType.Black;
        ShowMode = EWindowShowMode.HideOther;
    }

    protected override void OnAwake()
    {
        for (int i = 1; i <= 3; i++)
        {
            GameObject btn = transform.Find("Pivot/Boxs/" + i.ToString()).gameObject;
            boxBtns[i - 1] = btn;
            boxStates[i - 1] = btn.GetComponent<UISprite>();
            boxStarNums[i - 1] = btn.transform.Find("StarNum").GetComponent<UILabel>();
        }
        btnClose = transform.Find("Pivot/BtnClose").gameObject;
        btnPrev = transform.Find("Pivot/BtnPrev").gameObject;
        btnNext = transform.Find("Pivot/BtnNext").gameObject;

        chapterName = transform.Find("Pivot/ChapterName").GetComponent<UILabel>();
        copysParent = transform.Find("Pivot/Copys");

        for (int i = 1; i <= 6; i++)
        {
            ItemCopy item = new ItemCopy();
            GameObject go = copysParent.Find(i.ToString()).gameObject;
            item.item = go;
            for (int k = 1; k <= 3; k++)
            {
                item.stars[k - 1] = go.transform.Find("Star/Star" + k.ToString()).GetComponent<UISprite>();
            }
            item.icon = go.transform.Find("Icon").GetComponent<UISprite>();
            item.copyName = go.transform.Find("CopyName").GetComponent<UILabel>();
            copyItems.Add(item);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnBtnCloseClick;
        UIEventListener.Get(btnPrev).onClick = OnBtnPrevClick;
        UIEventListener.Get(btnNext).onClick = OnBtnNextClick;
        for (int i = 0; i < 3; i++)
        {
            UIEventListener.Get(boxBtns[i]).onClick = OnBoxClick;
        }
        for (int i = 0; i < copyItems.Count; i++)
        {
            UIEventListener.Get(copyItems[i].item).onClick = OnCopyClick;
        }
    }

    protected override void OnAddHandler()
    {

    }

    protected override void OnEnable()
    {
        int chapter = MLRaid.Instance.GetMaxChapterByDifficulty(copyDifficulty);
        ShowViewByChapter(chapter);
    }

    protected override void OnDelHandler()
    {

    }

    protected override void OnClose()
    {
        copyItems.Clear();
    }

    private void OnCopyClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        DCopyMainChapter db = ReadCfgCopyMainChapter.GetDataById(chapter);
        int enterIndex = MLRaid.Instance.GetMaxCanEnterCopyIndex(copyDifficulty, chapter);
        int index = go.name.ToInt32();
        if (index > enterIndex)
        {
            GTItemHelper.ShowTip("未通关前置关卡");
            return;
        }
        GTWindowManager.Instance.OpenWindow(EWindowID.UIMainCopy);
        UIMainCopy window = (UIMainCopy)GTWindowManager.Instance.GetWindow(EWindowID.UIMainCopy);
        window.ShowViewById(copyDifficulty, chapter, db.Copys[index - 1]);
    }

    private void OnBoxClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        int index = go.name.ToInt32() - 1;
        DCopyMainChapter db = ReadCfgCopyMainChapter.GetDataById(chapter);
        GTWindowManager.Instance.OpenWindow(EWindowID.UIAwardBox);
        UIAwardBox window = (UIAwardBox)GTWindowManager.Instance.GetWindow(EWindowID.UIAwardBox);
        ERewardState state = MLRaid.Instance.GetChapterRewardStateByAwardIndex(chapter, index);
        UIEventListener.VoidDelegate doReceive = delegate (GameObject w)
        {
            GTNetworkSend.Instance.TryMainCopyReceiveReward(chapter, index);
        };
        window.ShowAwardById(db.Awards[index], state, doReceive);
    }

    private void OnBtnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void OnBtnNextClick(GameObject go)
    {
        if (chapter < DCopyMainChapter.MAINCOPY_ED_CHAPTER_ID)
        {
            ShowViewByChapter(chapter + 1);
        }
    }

    private void OnBtnPrevClick(GameObject go)
    {
        if (chapter > DCopyMainChapter.MAINCOPY_ST_CHAPTER_ID)
        {
            ShowViewByChapter(chapter - 1);
        }
    }


    private void ShowViewByChapter(int chapter)
    {
        this.chapter = chapter;
        DCopyMainChapter chapterDB = ReadCfgCopyMainChapter.GetDataById(chapter);
        int enterIndex = MLRaid.Instance.GetMaxCanEnterCopyIndex(copyDifficulty, chapter);
        for (int i = 0; i < 3; i++)
        {
            boxStarNums[i].text = chapterDB.Stars[i].ToString();
            ERewardState rewardState = MLRaid.Instance.GetChapterRewardStateByAwardIndex(chapter, i);
            switch(rewardState)
            {
                case ERewardState.NOT_RECEIVE:
                case ERewardState.CAN_RECEIVE:
                    boxStates[i].spriteName = "613";
                    break;
                case ERewardState.HAS_RECEIVE:
                    boxStates[i].spriteName = "614";
                    break;
            }
        }
        chapterName.text = chapterDB.Name;
        for (int i = 0; i < copyItems.Count; i++)
        {
            ItemCopy tab = copyItems[i];
            if (i < chapterDB.Copys.Length)
            {
                tab.item.SetActive(true);
                Color color = i < enterIndex ? Color.white : Color.cyan;
                tab.icon.color = color;
                for (int k = 0; k < tab.stars.Length; k++)
                {
                    tab.stars[k].color = color;
                }
                int copyID = chapterDB.Copys[i];
                DCopy db = ReadCfgCopy.GetDataById(copyID);
                int starNum = MLRaid.Instance.GetCopyStarNumById(copyID);
                tab.copyName.text = db.Name;
                tab.copyName.color = i < enterIndex ? Color.yellow : Color.gray;
                tab.item.transform.localPosition = chapterDB.CopyPosArray[i];
                GTItemHelper.ShowStarNum(tab.stars, starNum);
            }
            else
            {
                copyItems[i].item.SetActive(false);
            }
        }
        btnPrev.SetActive(chapter > DCopyMainChapter.MAINCOPY_ST_CHAPTER_ID);
        btnNext.SetActive(chapter < DCopyMainChapter.MAINCOPY_ED_CHAPTER_ID);
    }
}
