using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIPet : GTWindow
{
    private GameObject        btnClose;
    private GameObject        btnUpgrade;
    private GameObject        btnBattle;
    private GameObject        btnUnload;
    private GameObject        btnAutoToAddFood;

    private UISlider          expSlider;
    private UILabel           expNum;

    private UIScrollView      petView;
    private UIGrid            petGrid;
    private UICenterOnChild   petCenterOnChild;
    private GameObject        petTemp;

    private UILabel           petCurrPropertyText1;
    private UILabel           petCurrPropertyText2;
    private UILabel           petMainPropertyText1;
    private UILabel           petMainPropertyText2;
    private UILabel           petDesc;
    private UILabel           petName;
    private UITexture         petTexture;

    private List<FoodItem>    mFoods = new List<FoodItem>();
    private List<TempItem>    mTemps = new List<TempItem>();
    private List<XItem>       mItems = new List<XItem>();
    private int               mCurPetID;
    private ERender           mRender;
    private CharacterAvatar   mAvatar;



    class FoodItem
    {
        public GameObject     itemBtn;
        public UITexture      itemTexture;
        public UISprite       itemQuality;
    }

    class TempItem
    {
        public int            id;
        public GameObject     btn;
        public GameObject     dress;
        public UIToggle       toggle;
        public UISprite       icon;
        public UISprite       quality;
        public UILabel        name;
    }

    public UIPet()
    {
        Type = EWindowType.Window;
        Resident = false;
        Path = "Pet/UIPet";
        MaskType = EWindowMaskType.Black;
        ShowMode = EWindowShowMode.HideOther;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        btnClose = pivot.Find("BtnClose").gameObject;
        btnUpgrade = pivot.Find("Bottom/BtnUpgrade").gameObject;
        btnBattle = pivot.Find("Bottom/BtnBattle").gameObject;
        btnUnload = pivot.Find("Bottom/BtnUnload").gameObject;
        btnAutoToAddFood = pivot.Find("Bottom/BtnAutoToAddFood").gameObject;

        Transform foodTrans = pivot.Find("Bottom/Foods");
        for (int i = 1; i <= 6; i++)
        {
            FoodItem tab = new FoodItem();
            Transform trans = foodTrans.Find(i.ToString());
            tab.itemBtn = trans.gameObject;
            tab.itemTexture = trans.Find("Texture").GetComponent<UITexture>();
            tab.itemQuality = trans.Find("Quality").GetComponent<UISprite>();
            mFoods.Add(tab);
        }

        expSlider = pivot.Find("Bottom/Progress").GetComponent<UISlider>();
        expNum = pivot.Find("Bottom/Progress/Num").GetComponent<UILabel>();

        petView = pivot.Find("View").GetComponent<UIScrollView>();
        petGrid = pivot.Find("View/Grid").GetComponent<UIGrid>();
        petCenterOnChild = pivot.Find("View/Grid").GetComponent<UICenterOnChild>();
        petCenterOnChild.enabled = false;
        petTemp = pivot.Find("View/Temp").gameObject;
        petTemp.SetActive(false);

        petCurrPropertyText1 = pivot.Find("1/Text1").GetComponent<UILabel>();
        petCurrPropertyText2 = pivot.Find("1/Text2").GetComponent<UILabel>();
        petMainPropertyText1 = pivot.Find("2/Text1").GetComponent<UILabel>();
        petMainPropertyText2 = pivot.Find("2/Text2").GetComponent<UILabel>();
        petDesc = pivot.Find("3/Desc").GetComponent<UILabel>();

        petTexture = pivot.Find("ModelTexture").GetComponent<UITexture>();
        InitItems();
        ShowRender();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnReturnClick;
        UIEventListener.Get(btnUpgrade).onClick = OnUpgradeClick;
        UIEventListener.Get(btnBattle).onClick = OnBattleClick;
        UIEventListener.Get(btnUnload).onClick = OnUnloadClick;
        UIEventListener.Get(btnAutoToAddFood).onClick = OnAutoToAddFoodClick;
        UIEventListener.Get(petTexture.gameObject).onDrag = OnDragModel;
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_PET_UPGRADE, OnRecvUpgrade);
        GTEventCenter.AddHandler(GTEventID.TYPE_PET_BATTLE, OnRecvBattlePet);
        GTEventCenter.AddHandler(GTEventID.TYPE_PET_UNLOAD, OnRecvUnLoadPet);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_PET_UPGRADE, OnRecvUpgrade);
        GTEventCenter.DelHandler(GTEventID.TYPE_PET_BATTLE, OnRecvBattlePet);
        GTEventCenter.DelHandler(GTEventID.TYPE_PET_UNLOAD, OnRecvUnLoadPet);
    }

    protected override void OnEnable()
    {
        ShowView();
        ShowListView();
    }

    protected override void OnClose()
    {
        mCurPetID = 0;
        mFoods.Clear();
        mItems.Clear();
        mTemps.Clear();
        ERender.DelRender(mRender);
        mRender = null;
    }

    private void OnDragModel(GameObject go, Vector2 delta)
    {
        if (mAvatar == null && mAvatar.GetRootObj() != null)
        {
            return;
        }
        ESpin.Get(mAvatar.GetRootObj()).OnSpin(delta, 2);
    }

    private void OnUnloadClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryUnloadPet(mCurPetID);
    }

    private void OnBattleClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        GTNetworkSend.Instance.TryBattlePet(mCurPetID);
    }

    private void OnAutoToAddFoodClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (mItems.Count > 0)
        {
            return;
        }
        MLPet.Instance.GetItemListToOneKeyUpgrade(ref mItems);
        if (mItems.Count == 0)
        {
            GTItemHelper.ShowTip("缺少食物");
            return;
        }
        ShowView();
    }

    private void OnUpgradeClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (mItems.Count == 0)
        {
            GTItemHelper.ShowTip("请添加食物");
            return;
        }
        GTNetworkSend.Instance.TryUpgradePet(mCurPetID, mItems);
    }

    private void OnReturnClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void InitItems()
    {
        Dictionary<int, DActor> dict = new Dictionary<int, DActor>();
        foreach(var current in ReadCfgActor.Dict)
        {
            if(current.Value.Type==EActorType.PET)
            {
                dict[current.Key] = current.Value;
            }
        }

        Dictionary<int, DActor>.Enumerator em = dict.GetEnumerator();
        int group = GTWindowManager.Instance.GetToggleGroupId();
        int num = 0;
        while (em.MoveNext())
        {
            GameObject go = NGUITools.AddChild(petGrid.gameObject, petTemp);
            petGrid.AddChild(go.transform);
            go.SetActive(true);

            int id = em.Current.Value.Id;
            go.name = id.ToString();

            TempItem tab = new TempItem();
            tab.id = id;
            tab.btn = go;
            tab.toggle= go.GetComponent<UIToggle>();
            tab.icon = go.transform.Find("Icon").GetComponent<UISprite>();
            tab.name = go.transform.Find("Name").GetComponent<UILabel>();
            tab.dress = go.transform.Find("Dress").gameObject;
            tab.quality = go.transform.Find("Quality").GetComponent<UISprite>();
            mTemps.Add(tab);

            num++;
            if (num == 1)
            {
                tab.toggle.value = true;
                mCurPetID = em.Current.Value.Id;
            }

            int index = num;

            UIEventListener.Get(go).onClick = delegate (GameObject obj)
            {
                GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
                if (index > 2 && index < dict.Count - 2)
                {
                    petCenterOnChild.enabled = true;
                    petCenterOnChild.CenterOn(go.transform);
                    petCenterOnChild.onCenter = delegate (GameObject centerObj)
                    {
                        petCenterOnChild.enabled = false;
                    };
                }
                Switch(id);
            };

        }
        em.Dispose();
        petGrid.repositionNow = true;
    }

    private void Switch(int id)
    {
        if (mCurPetID == id)
        {
            return;
        }
        mCurPetID = id;
        ShowRender();
        ShowView();
    }

    private void OnRecvUpgrade()
    {
        mItems.Clear();
        ShowView();
    }

    private void OnRecvUnLoadPet()
    {
        ShowView();
        ShowListView();
    }

    private void OnRecvBattlePet()
    {
        ShowView();
        ShowListView();
    }

    private void ShowRender()
    {
        DActor db = ReadCfgActor.GetDataById(mCurPetID);
        if (db == null)
        {
            mRender = ERender.DelRender(mRender);
            return;
        }
        if (mRender == null)
        {
            mRender = ERender.AddRender(petTexture);
        }
        mRender.DelAllModels();
        mAvatar = GTWorld.Instance.AddAvatar(db.Model);
        mAvatar.PlayAnim("idle", null);
        GameObject model = mRender.AddModel(mAvatar.GetRootObj());
        DStage ds = ReadCfgStagePet.GetDataById(mCurPetID);
        model.transform.localPosition = new Vector3(ds.X, ds.Y, ds.Z);
        model.transform.localEulerAngles = new Vector3(0, 120, 0);
        model.transform.localScale = Vector3.one * ds.Scale;
    }

    private void ShowListView()
    {
        XCharacter role = GTGlobal.Main;
        for (int i = 0; i < mTemps.Count; i++)
        {
            TempItem tab = mTemps[i];
            DActor db = ReadCfgActor.GetDataById(tab.id);
            GTItemHelper.ShowQualityText(tab.name, db.Name, db.Quality);
            GTItemHelper.ShowActorQuality(tab.quality, db.Id);
            tab.icon.spriteName = db.Icon;
            tab.dress.SetActive(tab.id == role.PetID);
        }
    }

    private void ShowView()
    {
        DActor db = ReadCfgActor.GetDataById(mCurPetID);
        XPet data = DataDBSPet.GetDataById(mCurPetID);
        data = data == null ? GTDataFactory.CreatePet(mCurPetID) : data;
        DPetLevel levelDB = ReadCfgPetLevel.GetDataById(db.Quality * 1000 + data.Level);
        GTItemHelper.ShowProgressSlider(expSlider, data.CurExp, levelDB.Exp);
        GTItemHelper.ShowProgressText(expNum, data.CurExp, levelDB.Exp);
        for (int i = 0; i < mFoods.Count; i++)
        {
            FoodItem tab = mFoods[i];
            if (i < mItems.Count)
            {
                XItem itemData = mItems[i];
                GTItemHelper.ShowItemTexture(tab.itemTexture, itemData.Id);
                GTItemHelper.ShowItemQuality(tab.itemQuality, itemData.Id);
            }
            else
            {
                tab.itemQuality.gameObject.SetActive(false);
                tab.itemTexture.gameObject.SetActive(false);
            }
        }
        string str = GTTools.Format("{0}    +{1}", db.Name, data.Level);
        GTItemHelper.ShowQualityText(petName, str, db.Quality);


        ShowPropertyView(mCurPetID, data.Level);
        XCharacter role = GTGlobal.Main;

        btnBattle.SetActive(role.PetID != mCurPetID);
        btnUnload.SetActive(role.PetID == mCurPetID);
    }

    private void ShowPropertyView(int id, int level)
    {
        DActor db = ReadCfgActor.GetDataById(id);
        petCurrPropertyText1.text = string.Empty;
        petCurrPropertyText2.text = string.Empty;
        petMainPropertyText1.text = string.Empty;
        petMainPropertyText2.text = string.Empty;

        DPetLevel levelDB = ReadCfgPetLevel.GetDataById(db.Quality * 1000 + level);
        for (int i = 0; i < levelDB.PropertyNums.Length; i++)
        {
            EAttr propertyID = (EAttr)(i + 1);
            DProperty propertyDB = ReadCfgProperty.GetDataById(propertyID);
            petCurrPropertyText1.Append(propertyDB.Name);
            petCurrPropertyText2.Append("+" + levelDB.PropertyNums[i].ToString());

            petMainPropertyText1.Append(propertyDB.Name);
            int extendNum = (int)(levelDB.PropertyNums[i] * levelDB.Ratio / 100f);
            petMainPropertyText2.Append("+" + extendNum.ToString());
        }

    }

}
