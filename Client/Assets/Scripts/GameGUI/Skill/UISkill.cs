using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UISkill : GTWindow
{
    private GameObject         btnLearnTalent;
    private GameObject         btnClose;
    private UIToggle[]         mMenus;
    private int                mCurFunc = 0;

    private Transform          mContent1;
    private Transform          mContent2;
    private Transform          mContent3;

    private UIScrollView       mContentScollView1;
    private UIGrid             mContentGrid1;
    private GameObject         mContentTemp1;

    private UIScrollView       mContentScollView2;
    private UIGrid             mContentGrid2;
    private GameObject         mContentTemp2;


    private UIScrollView       mContentScollView3;
    private UIGrid             mContentGrid3;
    private GameObject         mContentTemp3;
    private List<ItemTalent>   mTalents = new List<ItemTalent>();

    class ItemTalent
    {
        public UILabel         talentLevel;
        public UISprite        talentLevelBack;
        public GameObject[]    talentBtns;
        public UISprite  []    talentBacks;
        public GameObject[]    talentHighLights;
        public UILabel   []    talentNames;
        public UITexture []    talentIcons;
    }

    public UISkill()
    {
        Type = EWindowType.Window;
        Path = "Skill/UISkill";
        MaskType = EWindowMaskType.Black;
        ShowMode = EWindowShowMode.HideOther;
        Resident = false;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnClose = pivot.Find("BtnClose").gameObject;
        mMenus = new UIToggle[3];
        mMenus[0] = pivot.Find("Left/Menus/Menu_Skill").GetComponent<UIToggle>();
        mMenus[1] = pivot.Find("Left/Menus/Menu_Talent").GetComponent<UIToggle>();
        mMenus[2] = pivot.Find("Left/Menus/Menu_SkillSet").GetComponent<UIToggle>();
        int group = GTWindowManager.Instance.GetToggleGroupId();
        for (int i = 0; i < mMenus.Length; i++)
        {
            mMenus[i].group = group;
        }
        mMenus[mCurFunc].value = true;

        mContent1 = pivot.Find("Content/Content1");
        mContent2 = pivot.Find("Content/Content2");
        mContent3 = pivot.Find("Content/Content3");

        mContentScollView1 = mContent1.Find("View").GetComponent<UIScrollView>();
        mContentScollView2 = mContent2.Find("View").GetComponent<UIScrollView>();
        mContentScollView3 = mContent3.Find("View").GetComponent<UIScrollView>();
        mContentGrid1 = mContent1.Find("View/Grid").GetComponent<UIGrid>();
        mContentGrid2 = mContent2.Find("View/Grid").GetComponent<UIGrid>();
        mContentGrid3 = mContent3.Find("View/Grid").GetComponent<UIGrid>();
        mContentTemp1 = mContent1.Find("View/Temp").gameObject;
        mContentTemp2 = mContent2.Find("View/Temp").gameObject;
        mContentTemp3 = mContent3.Find("View/Temp").gameObject;
        btnLearnTalent = mContent2.Find("Btn_Learn").gameObject;
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick            = OnReturnClick;
        UIEventListener.Get(btnLearnTalent).onClick       = OnLearnTalentClick;
        UIEventListener.Get(mMenus[0].gameObject).onClick = OnMenuSkillClick;
        UIEventListener.Get(mMenus[1].gameObject).onClick = OnMenuTalentClick;
        UIEventListener.Get(mMenus[2].gameObject).onClick = OnMenuSkillSetClick;
    }

    protected override void OnAddHandler()
    {

    }

    protected override void OnEnable()
    {
        InitContentView2();
        ShowContentView2();
    }

    protected override void OnDelHandler()
    {

    }

    protected override void OnClose()
    {
        mTalents.Clear();
    }

    private void OnMenuSkillSetClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnMenuTalentClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnMenuSkillClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnLearnTalentClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnReturnClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void InitContentView2()
    {
        for (int i = 0; i < DSkillTalent.TALENT_LEVELS.Length; i++)
        {
            GameObject item = NGUITools.AddChild(mContentGrid2.gameObject, mContentTemp2, i == 0);
            mContentGrid2.AddChild(item.transform);
            item.SetActive(true);
            ItemTalent tab = new ItemTalent();
            tab.talentLevel = item.transform.Find("Level/Num").GetComponent<UILabel>();
            tab.talentLevelBack = item.transform.Find("Level").GetComponent<UISprite>();
            tab.talentBacks = new UISprite[3];
            tab.talentHighLights = new GameObject[3];
            tab.talentNames = new UILabel[3];
            tab.talentBtns = new GameObject[3];
            tab.talentIcons = new UITexture[3];
            for (int k = 1; k <= 3; k++)
            {
                Transform trans = item.transform.Find(k.ToString());
                tab.talentBtns[k - 1] = trans.gameObject;
                tab.talentBacks[k - 1] = trans.Find("Back").GetComponent<UISprite>();
                tab.talentHighLights[k - 1] = trans.Find("HighLight").gameObject;
                tab.talentNames[k - 1] = trans.Find("Name").GetComponent<UILabel>();
                tab.talentIcons[k - 1] = trans.Find("Icon").GetComponent<UITexture>();
            }
            mTalents.Add(tab);
        }
    }

    private void ShowContentView2()
    {
        Dictionary<int, DSkillTalent> dict = ReadCfgSkillTalent.Dict;
        Dictionary<int, DSkillTalent>.Enumerator em = dict.GetEnumerator();
        XCharacter role = GTGlobal.Main;
        int[] activeArray = new int[mTalents.Count];
        for (int i = 0; i < mTalents.Count; i++)
        {
            ItemTalent tab = mTalents[i];
            tab.talentLevel.text = DSkillTalent.TALENT_LEVELS[i].ToString();
            GTItemHelper.ShowImageBlack(tab.talentLevelBack, role.Level < DSkillTalent.TALENT_LEVELS[i]);
            activeArray[i] = 0;
        }
        while (em.MoveNext())
        {
            if (GTGlobal.CurPlayerCarrerID != em.Current.Value.Carrer)
            {
                continue;
            }
            DSkillTalent db = em.Current.Value;
            int layer = em.Current.Value.Layer;
            int i = db.Pos - 1;
            ItemTalent tab = mTalents[layer - 1];
            GTItemHelper.ShowTexture(tab.talentIcons[i], db.Icon);
            tab.talentNames[i].text = db.Name.ToString();
            tab.talentHighLights[i].SetActive(activeArray[layer - 1] == db.Pos);
        }
        em.Dispose();
    }
}
