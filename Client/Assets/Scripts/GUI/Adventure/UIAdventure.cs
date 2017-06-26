using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;

public class UIAdventure : GTWindow
{
    private GameObject    btnReturn;
    private GameObject    btnChallenge;
    private UIScrollView  mScrollView;
    private UIWrapContent mWrapContent;
    private int           mCurAdventureIndex = 0;

    public UIAdventure()
    {
        Type = EWindowType.WINDOW;
        mResPath = "Adventure/UIAdventure";
    }

    protected override void OnAwake()
    {
        btnReturn = transform.Find("Top/Btn_Return").gameObject;
        btnChallenge = transform.Find("Bottom/Btn_Challenge").gameObject;
        mScrollView = transform.Find("View").GetComponent<UIScrollView>();
        mWrapContent = transform.Find("View/Grid").GetComponent<UIWrapContent>();
    }

    private void InitView()
    {
        Dictionary<int, DAdventure> dict = ReadCfgAdventure.Dict;
        mWrapContent.onInitItem = OnInitItem;
        mWrapContent.SetRealCount(dict.Count, UIWrapContent.Pivot.Left);
    }

    private void ShowSelect(int index)
    {
        for(int i=0;i<mWrapContent.transform.childCount;i++)
        {
            Transform child = mWrapContent.transform.GetChild(i);
            child.Find("HighLight").gameObject.SetActive(false);
        }
        Transform trans = mWrapContent.FindItem(mCurAdventureIndex);
        if(trans!=null)
        {
            trans.Find("HighLight").gameObject.SetActive(true);
        }
    }

    private void OnInitItem(Transform trans, int index)
    {
        int id = index + 1;
        DAdventure db = ReadCfgAdventure.GetDataById(id);
        trans.Find("Name").GetComponent<UILabel>().text = db.Name;
        trans.Find("Icon").GetComponent<UISprite>().spriteName = db.Icon;
        if(db.Times<=0)
        {
            trans.Find("Times").GetComponent<UILabel>().text = string.Empty;
        }
        else
        {
            string localString = GTItemHelper.GetText("次数：{0}/{1}");
            trans.Find("Times").GetComponent<UILabel>().text = GTTools.Format(localString, db.Times, db.Times); 
        }
        UIEventListener.Get(trans.gameObject).onClick = delegate (GameObject go)
        {
            GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
            if (mCurAdventureIndex == index)
                return;
            this.mCurAdventureIndex = index;
            ShowSelect(mCurAdventureIndex);
        };
        ShowSelect(index);
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnReturn).onClick = OnReturnClick;
        UIEventListener.Get(btnChallenge).onClick = OnChallengeClick;
    }

    private void OnChallengeClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
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
        this.InitView();
    }

    protected override void OnDelHandler()
    {
        
    }

    protected override void OnClose()
    {
        
    }
}
