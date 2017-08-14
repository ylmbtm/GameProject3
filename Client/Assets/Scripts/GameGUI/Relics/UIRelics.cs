using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIRelics : GTWindow
{
    private Transform            cardTrans;
    private GameObject           cardTemp;
    private GameObject           cardDrag;
    private GameObject           btnClose;
    private GameObject           btnUpgrade;
    private GameObject           btnBattle;
    private GameObject           btnSkill;
    private GameObject           btnUnload;
    private GameObject           view1;
    private GameObject           view2;
    private UILabel              relicsName;
    private UILabel              relicsDesc;

    private List<RelicsCard>     mCards         = new List<RelicsCard>();
    private List<RelicsMove>     mMoves         = new List<RelicsMove>();
    private List<int>            mMoveList      = new List<int>();
    private List<RelicsCharge>   mRelicsCharges = new List<RelicsCharge>();
    private List<RelicsProperty> mNowPropertys  = new List<RelicsProperty>();
    private List<RelicsProperty> mNexPropertys  = new List<RelicsProperty>();

    private GameObject           nowAttr;
    private GameObject           nexAttr;
    private GameObject           attrRow;

    private float                changeX      = 80;
    private float                changeScale  = 0.1f;
    private bool                 mIsDraging   = false;
    private int                  mCenterIndex = 0;
    private int                  mOpenFunc    = 1;
    private ERender              mRender;
    private GameObject           mModel;
    private UITexture            mModelTexture;

    private int centerID
    {
        get
        {
            if (mCards != null && mCards.Count >= mCenterIndex + 1)
            {
                return mCards[mCenterIndex].id;
            }
            return 0;
        }
    }

    class RelicsCharge
    {
        public UILabel    itemName;
        public UILabel    itemNum;
        public UILabel    tip;
        public GameObject btn;
        public UITexture  itemTexture;
        public UISprite   itemQuality;
        public UISlider   chargeProgressValue;
        public UILabel    chargeProgressNum;
    }

    class RelicsProperty
    {
        public UILabel    propertyName;
        public UILabel    propertyNum;
    }

    class RelicsCard
    {
        public UITexture  icon;
        public UILabel    progress;
        public UISlider   progressBar;
        public UILabel    name;
        public GameObject btn;
        public Transform  trans;
        public int        id;
    }

    class RelicsMove
    {
        public Vector3    pos;
        public Vector3    scale;
        public int        depth;
    }

    public UIRelics()
    {
        Type = EWindowType.Window;
        Resident = false;
        Path = "Relics/UIRelics";
        MaskType = EWindowMaskType.Black;
        ShowMode = EWindowShowMode.HideOther;
        InitDepthAndPosition();
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        view1 = pivot.Find("View1").gameObject;
        view2 = pivot.Find("View2").gameObject;
        relicsName = pivot.Find("RelicsName").GetComponent<UILabel>();
        relicsDesc = pivot.Find("RelicsDesc").GetComponent<UILabel>();
        cardTrans = pivot.Find("View1/Cards");
        cardTemp  = pivot.Find("View1/Card").gameObject;
        cardTemp.SetActive(false);
        cardDrag  = pivot.Find("Drag").gameObject;
        btnClose = pivot.Find("BtnClose").gameObject;
        btnUpgrade = pivot.Find("View2/Btn_Upgrade").gameObject;
        btnSkill = pivot.Find("View2/Btn_Skill").gameObject;
        btnUnload = pivot.Find("View2/Btn_Unload").gameObject;
        btnBattle = pivot.Find("View2/Btn_Battle").gameObject;
        Transform trans = view2.transform.Find("Items");
        for (int i = 1; i <= 3; i++)
        {
            RelicsCharge tab = new RelicsCharge();
            Transform item = trans.Find(i.ToString());
            tab.itemQuality = item.Find("Quality").GetComponent<UISprite>();
            tab.itemTexture = item.Find("Texture").GetComponent<UITexture>();
            tab.itemNum = item.Find("Num").GetComponent<UILabel>();
            tab.tip = item.Find("Tip").GetComponent<UILabel>();
            tab.btn = item.Find("Btn").gameObject;
            tab.itemName = item.Find("Name").GetComponent<UILabel>();
            tab.chargeProgressNum = item.Find("Progress/Num").GetComponent<UILabel>();
            tab.chargeProgressValue = item.Find("Progress").GetComponent<UISlider>();
            mRelicsCharges.Add(tab);
        }
        Transform propertyTrans = view2.transform.Find("RelicsProperty");
        Transform tr1 = propertyTrans.Find("1");
        nowAttr = tr1.gameObject;
        for (int i = 1; i <= 3; i++)
        {
            RelicsProperty tab = new RelicsProperty();
            tab.propertyName = tr1.Find(i + "/Name").GetComponent<UILabel>();
            tab.propertyNum = tr1.Find(i + "/Num").GetComponent<UILabel>();
            mNowPropertys.Add(tab);
        }
        Transform tr2 = propertyTrans.Find("2");
        nexAttr = tr2.gameObject;
        for (int i = 1; i <= 3; i++)
        {
            RelicsProperty tab = new RelicsProperty();
            tab.propertyName = tr2.Find(i + "/Name").GetComponent<UILabel>();
            tab.propertyNum = tr2.Find(i + "/Num").GetComponent<UILabel>();
            mNexPropertys.Add(tab);
        }
        attrRow = propertyTrans.Find("Row").gameObject;
        mModelTexture = view2.transform.Find("RelicsTexture").GetComponent<UITexture>();
        CreateItems();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(cardDrag).onDrag = MoveOnDrag;
        UIEventListener.Get(cardDrag).onClick = OnCenterClick;
        UIEventListener.Get(btnClose).onClick = OnReturnClick;
        UIEventListener.Get(btnUpgrade).onClick = OnUpgradeClick;
        UIEventListener.Get(btnSkill).onClick = OnSkillClick;
        UIEventListener.Get(btnUnload).onClick = OnUnloadClick;
        UIEventListener.Get(btnBattle).onClick = OnBattleClick;
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler<int, int>(GTEventID.TYPE_RELICS_CHARGE, OnRecvChargeRelics);
        GTEventCenter.AddHandler<int>(GTEventID.TYPE_RELICS_UPGRADE, OnRecvUpgradeRelics);
        GTEventCenter.AddHandler<int>(GTEventID.TYPE_RELICS_BATTLE, OnRecvBattleRelics);
        GTEventCenter.AddHandler<int>(GTEventID.TYPE_RELICS_UNLOAD, OnRecvUnloadRelics);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler<int, int>(GTEventID.TYPE_RELICS_CHARGE, OnRecvChargeRelics);
        GTEventCenter.DelHandler<int>(GTEventID.TYPE_RELICS_UPGRADE, OnRecvUpgradeRelics);
        GTEventCenter.DelHandler<int>(GTEventID.TYPE_RELICS_BATTLE, OnRecvBattleRelics);
        GTEventCenter.DelHandler<int>(GTEventID.TYPE_RELICS_UNLOAD, OnRecvUnloadRelics);
    }

    protected override void OnEnable()
    {
        ShowView();
    }

    protected override void OnClose()
    {
        mCards.Clear();
        mMoveList.Clear();
        mRelicsCharges.Clear();
        mNowPropertys.Clear();
        mNexPropertys.Clear();
        mCenterIndex = 0;
        mOpenFunc = 1;
        ERender.DelRender(mRender);
        mRender = null;
    }

    private void CreateItems()
    {
        Dictionary<int, DRelics> dict = ReadCfgRelics.Dict;
        Dictionary<int, DRelics>.Enumerator em = dict.GetEnumerator();
        int i = 0;
        while (em.MoveNext())
        {
            GameObject go = NGUITools.AddChild(cardTrans.gameObject, cardTemp);
            DRelics db = em.Current.Value;
            go.name = i.ToString();
            go.SetActive(true);

            RelicsCard tab = new RelicsCard();
            tab.btn = go;
            tab.trans = go.transform;
            tab.name = go.transform.Find("Name").GetComponent<UILabel>();
            tab.progressBar = go.transform.Find("ProgressBar").GetComponent<UISlider>();
            tab.progress = go.transform.Find("ProgressBar/Num").GetComponent<UILabel>();
            tab.icon = go.transform.Find("Texture").GetComponent<UITexture>();
            tab.id = em.Current.Key;

            tab.name.text = db.Name;
            tab.progressBar.value = 0;
            tab.progress.text = "0%";
            GTItemHelper.ShowTexture(tab.icon, db.Icon);

            mMoveList.Add(i);
            mCards.Add(tab);   
            i++;
        }
        em.Dispose();
    }

    private void InitDepthAndPosition()
    {
        for (int i = 0; i < 10; i++)
        {
            RelicsMove tab = new RelicsMove();
            if (i < 5)
            {
                tab.pos = new Vector3(changeX * i, 0, 0);
                tab.scale = Vector3.one * (1 - changeScale * i);
                tab.depth = 500 - i * 100;
            }
            else if (i == 5)
            {
                tab.pos = Vector3.zero;
                tab.scale = new Vector3(0.01f, 0.01f, 0.01f);
                tab.depth = 0;
            }
            else
            {
                tab.pos = new Vector3(-changeX * (10 - i), 0, 0);
                tab.scale = Vector3.one * (1 - changeScale * (10 - i));
                tab.depth = (i - 5) * 100;
            }
            mMoves.Add(tab);
        }
    }

    private void ShowDepthAndPosition()
    {
        for(int i=0;i<mCards.Count;i++)
        {
            RelicsCard card = mCards[i];
            int index = mMoveList[i];
            RelicsMove move = mMoves[index];
            card.trans.localPosition = move.pos;
            card.trans.localScale =move.scale;
            SortDepth(move.depth, card.trans);
        }
    }

    private void ShowRelicsObj()
    {
        DRelics db = ReadCfgRelics.GetDataById(centerID);
        if (mRender == null)
        {
            mRender = ERender.AddRender(mModelTexture);
        }
        mRender.DelAllModels();
        mModel = mRender.AddModel(db.Model);
        if (mModel == null)
        {
            return;
        }
        mModel.SetActive(true);

        DStage ds = ReadCfgStageRelics.GetDataById(centerID);
        mModel.transform.localPosition = new Vector3(ds.X, ds.Y, ds.Z);
        mModel.transform.localEulerAngles = new Vector3(0, 180, 0);
        mModel.transform.localScale = Vector3.one;
        GameObject effect = GTWorld.Instance.Ect.LoadEffectObj(db.ActiveEffectID);
        if (effect == null)
            return;
        mRender.AddModel(effect);
        effect.transform.parent = mModel.transform;
        effect.transform.localPosition = Vector3.zero;
        effect.transform.localRotation = Quaternion.identity;
        effect.transform.localScale = Vector3.one;
    }

    private void MoveOnDrag(GameObject go,Vector2 delta)
    {
        if (mIsDraging == true || mOpenFunc == 2)
        {
            return;
        }
        mIsDraging = true;
        int dir = 0;
        if (delta.x > 0)
        {
            dir = 1;
            for (int i = 0; i < mMoveList.Count; i++)
            {
                mMoveList[i] = (mMoveList[i] == mMoveList.Count - 1) ? 0 : (mMoveList[i] + 1);
            }
            mCenterIndex = (mCenterIndex == 0) ? mMoveList.Count - 1 : (--mCenterIndex);
        }
        else if (delta.x < 0)
        {
            dir = -1;
            for (int i = 0; i < mMoveList.Count; i++)
            {
                mMoveList[i] = (mMoveList[i] == 0) ? (mMoveList.Count - 1) : (mMoveList[i] - 1);
            }
            mCenterIndex = (mCenterIndex == mMoveList.Count - 1) ? 0 : (++mCenterIndex);
        }
        if (dir != 0)
        {
            PlayMoveTween();
        }
    }

    private void OnCenterClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (mOpenFunc==2)
        {
            return;
        }
        mOpenFunc = 2;
        ShowView();
        ShowRelicsObj();
    }

    private void OnReturnClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        if (mOpenFunc == 1)
        {
            Hide();
        }
        else
        {
            mOpenFunc = 1;
            ShowView();
        }      
    }

    private void OnUpgradeClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryUpgradeRelics(centerID);
    }

    private void OnBattleClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryBattleRelics(centerID);
    }

    private void OnUnloadClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryUnloadRelics(centerID);
    }

    private void OnSkillClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTItemHelper.ShowTip("没有开放这个功能");
    }

    private void PlayMoveTween()
    {
        for (int i=0;i<mCards.Count;i++)
        {
            int index = mMoveList[i];
            RelicsMove move = mMoves[index];
            TweenPosition tp = TweenPosition.Begin(mCards[i].btn, 0.3f, move.pos);
            TweenScale.Begin(mCards[i].btn, 0.3f, move.scale);
        }
        mCards[0].btn.GetComponent<TweenPosition>().SetOnFinished(new EventDelegate(OnMoveTweenFinished));
    }

    private void OnMoveTweenFinished()
    {
        mIsDraging = false;
        ShowView();
    }

    private void SortDepth(int depth,Transform trans)
    {
        List<UIWidget> widgets = new List<UIWidget>();
        trans.GetComponents(widgets);
        trans.GetComponentsInChildren(widgets);
        widgets.Sort(Compare);
        for(int i=0;i<widgets.Count;i++)
        {
            widgets[i].depth = depth + i;
        }
    }

    private int Compare(UIWidget a, UIWidget b)
    {
        return a.depth - b.depth;
    }

    private void ShowView1()
    {
        ShowCurrRelicsView1();
        ShowDepthAndPosition();
    }

    private void ShowCurrRelicsView1()
    {
        DRelics db = ReadCfgRelics.GetDataById(centerID);
        XRelics relics = DataDBSRelics.GetDataById(centerID);
        relicsName.text = (relics == null) ? db.Name : GTTools.Format("{0}  +{1}", db.Name, relics.Level);
        relicsDesc.text = db.Desc.ToString();
    }

    private void ShowView2()
    {
        ShowCurrRelicsView2();
        ShowPropertyView();
        ShowButtonsState();
    }

    private void ShowCurrRelicsView2()
    {
        int targetID = mCards[mCenterIndex].id;
        DRelics db = ReadCfgRelics.GetDataById(targetID);
        XRelics relics = DataDBSRelics.GetDataById(targetID);

        for (int i = 0; i < mRelicsCharges.Count; i++)
        {
            RelicsCharge tab = mRelicsCharges[i];
            int itemID = db.ArtificeCostID[i];
            DItem itemDB = ReadCfgItem.GetDataById(itemID);
            GTItemHelper.ShowItemName(tab.itemName, itemID);
            GTItemHelper.ShowItemTexture(tab.itemTexture, itemID);
            GTItemHelper.ShowItemQuality(tab.itemQuality, itemID);
            tab.itemNum.text = GTTools.Format("当前拥有：[d96016]{0}[-]", GTDataManager.Instance.GetItemCountById(itemID));
            tab.tip.text = GTTools.Format("充能一次加 [d96916]{0}[-] 点经验", itemDB.Data1);

            if (relics == null)
            {
                tab.chargeProgressValue.value = 0;
                tab.chargeProgressNum.text = GTTools.Format("{0}/{1}", 0, db.LevelExp[0]);
            }
            else
            {
                int curExp = 0;
                switch (i + 1)
                {
                    case 1:
                        curExp = relics.CurExp1;
                        break;
                    case 2:
                        curExp = relics.CurExp2;
                        break;
                    case 3:
                        curExp = relics.CurExp3;
                        break;
                }

                int maxExp = db.LevelExp[relics.Level];
                GTItemHelper.ShowProgressSlider(tab.chargeProgressValue, curExp, maxExp);
                tab.chargeProgressNum.text = GTTools.Format("{0}/{1}", curExp, maxExp);
            }

            int index = i + 1;
            UIEventListener.Get(tab.btn).onClick = delegate (GameObject go)
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                GTNetworkSend.Instance.TryChargeRelics(targetID, index);
            };
        }
    }

    private void ShowPropertyView()
    {
        DRelics db = ReadCfgRelics.GetDataById(centerID);
        XRelics relics = DataDBSRelics.GetDataById(centerID);

        for (int i = 0; i < 3; i++)
        {
            RelicsProperty tab1 = mNowPropertys[i];
            RelicsProperty tab2 = mNexPropertys[i];
            DProperty propertyDB = ReadCfgProperty.GetDataById(db.PropertyID[i]);
            tab1.propertyName.text = propertyDB.Name;
            tab2.propertyName.text = propertyDB.Name.ToString();

            float ratio1 = 1;
            float ratio2 = ratio1 * (20 / 100f + 1);
            float num1 = 0;
            float num2 = 0;

            if (relics == null)
            {
                num1 = ratio1 * db.PropertyNum[i];
                num2 = ratio2 * db.PropertyNum[i];
                if (propertyDB.IsPercent)
                {
                    tab1.propertyNum.text = "+" + (num1 / 100f).ToPercent();
                    tab2.propertyNum.text = "+" + (num2 / 100f).ToPercent();
                }
                else
                {
                    tab1.propertyNum.text = "+" + ((int)num1).ToString();
                    tab2.propertyNum.text = "+" + ((int)num2).ToString();
                }
            }
            else
            {
                ratio1 = Mathf.Pow(20 / 100f + 1, relics.Level + 1);
                ratio2 = ratio1 * (20 / 100f + 1);

                num1 = (db.PropertyNum[i] * ratio1);
                num2 = (db.PropertyNum[i] * ratio2);

                if (relics.Level == db.LevelExp.Length)
                {
                    if (propertyDB.IsPercent)
                    {
                        tab1.propertyNum.text = "+" + (num1 / 100f).ToPercent();
                        tab2.propertyNum.text = string.Empty;
                    }
                    else
                    {
                        tab1.propertyNum.text = "+" + ((int)num1).ToString();
                        tab2.propertyNum.text = string.Empty;
                    }

                    nowAttr.transform.localPosition = new Vector3(100, -50, 0);
                    nexAttr.SetActive(false);
                    attrRow.SetActive(false);
                }
                else
                {
                    if (propertyDB.IsPercent)
                    {
                        tab1.propertyNum.text = "+" + (num1 / 100f).ToPercent();
                        tab2.propertyNum.text = "+" + (num2 / 100f).ToPercent();
                    }
                    else
                    {
                        tab1.propertyNum.text = "+" + ((int)num1).ToString();
                        tab2.propertyNum.text = "+" + ((int)num2).ToString();
                    }
                    nexAttr.SetActive(true);
                    attrRow.SetActive(true);
                    nowAttr.transform.localPosition = new Vector3(-20, -60, 0);
                    nexAttr.transform.localPosition = new Vector3(200, -60, 0);
                }
            }

        }
    }

    private void ShowButtonsState()
    {
        DRelics db = ReadCfgRelics.GetDataById(centerID);
        XRelics relics = DataDBSRelics.GetDataById(centerID);
        bool canUpgrade = true;
        if (relics != null)
        {
            for (int i = 0; i < 3; i++)
            {
                int curExp = 0;
                switch (i + 1)
                {
                    case 1:
                        curExp = relics.CurExp1;
                        break;
                    case 2:
                        curExp = relics.CurExp2;
                        break;
                    case 3:
                        curExp = relics.CurExp3;
                        break;
                }
                if (curExp < db.LevelExp[relics.Level])
                {
                    canUpgrade = false;
                }
            }
        }
        else
        {
            canUpgrade = false;
        }
        GTItemHelper.ShowImageBlack(btnUpgrade.transform.GetComponent<UISprite>(), !canUpgrade);


        XCharacter role = GTGlobal.Main;
        btnBattle.SetActive(role.Relic != centerID);
        btnUnload.SetActive(role.Relic == centerID);
    }

    private void ShowView()
    {
        view1.SetActive(mOpenFunc == 1);
        view2.SetActive(mOpenFunc == 2);
        if (mOpenFunc==1)
        {
            ShowView1();
        }
        else
        {
            ShowView2();
        }
    }

    private void OnRecvBattleRelics(int relicsID)
    {
        ShowView();
    }

    private void OnRecvUnloadRelics(int relicsID)
    {
        ShowView();
    }

    private void OnRecvChargeRelics(int arg1, int arg2)
    {
        ShowView();
    }

    private void OnRecvUpgradeRelics(int arg1)
    {
        ShowView();
    }
}
