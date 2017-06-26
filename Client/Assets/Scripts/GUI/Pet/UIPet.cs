using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIPet : GTWindow
{
    public UIPet()
    {
        Type = EWindowType.WINDOW;
        mResident = false;
        mResPath = "Pet/UIPet";
    }

    private GameObject btnReturn;
    private GameObject btnUpgrade;
    private GameObject btnBattle;
    private GameObject btnUnload;
    private GameObject btnAutoToAddFood;

    private UISlider expSlider;
    private UILabel  expNum;

    private UIScrollView    leftView;
    private UIGrid          leftGrid;
    private UICenterOnChild leftCenterOnChild;
    private GameObject      leftTemp;

    private UILabel currPropertyText1;
    private UILabel currPropertyText2;

    private UILabel mainPropertyText1;
    private UILabel mainPropertyText2;
    private UILabel desc;
    private UILabel title;

    private List<FoodItem> mFoods = new List<FoodItem>();
    private List<TempItem> mTemps = new List<TempItem>();
    private List<XItem>    mItems = new List<XItem>();

    private int             mCurPetID;
    private ERender         mRender;
    private CharacterAvatar mAvatar;
    private UITexture       mModelTexture;


    class FoodItem
    {
        public GameObject itemBtn;
        public UITexture itemTexture;
        public UISprite itemQuality;
    }

    class TempItem
    {
        public int id;
        public GameObject btn;
        public GameObject dress;
        public UIToggle toggle;
        public UISprite icon;
        public UISprite quality;
        public UILabel name;
    }

    protected override void OnAwake()
    {
        btnReturn = transform.Find("Top/Btn_Return").gameObject;
        btnUpgrade = transform.Find("Bottom/Btn_Upgrade").gameObject;
        btnBattle = transform.Find("Bottom/Btn_Battle").gameObject;
        btnUnload = transform.Find("Bottom/Btn_Unload").gameObject;
        btnAutoToAddFood = transform.Find("Bottom/Btn_AutoToAddFood").gameObject;
        title = transform.Find("Top/Title").GetComponent<UILabel>();

        Transform foodTrans = transform.Find("Bottom/Foods");
        for (int i = 1; i <= 6; i++)
        {
            FoodItem tab = new FoodItem();
            Transform trans = foodTrans.Find(i.ToString());
            tab.itemBtn = trans.gameObject;
            tab.itemTexture = trans.Find("Texture").GetComponent<UITexture>();
            tab.itemQuality = trans.Find("Quality").GetComponent<UISprite>();
            mFoods.Add(tab);
        }

        expSlider = transform.Find("Bottom/Progress").GetComponent<UISlider>();
        expNum = transform.Find("Bottom/Progress/Num").GetComponent<UILabel>();

        leftView = transform.Find("Left/View").GetComponent<UIScrollView>();
        leftGrid = transform.Find("Left/View/Grid").GetComponent<UIGrid>();
        leftCenterOnChild = transform.Find("Left/View/Grid").GetComponent<UICenterOnChild>();
        leftCenterOnChild.enabled = false;
        leftTemp = transform.Find("Left/View/Temp").gameObject;
        leftTemp.SetActive(false);

        Transform right = transform.Find("Right/Background");
        currPropertyText1 = right.Find("1/Text1").GetComponent<UILabel>();
        currPropertyText2 = right.Find("1/Text2").GetComponent<UILabel>();
        mainPropertyText1 = right.Find("2/Text1").GetComponent<UILabel>();
        mainPropertyText2 = right.Find("2/Text2").GetComponent<UILabel>();
        desc = right.Find("3/Desc").GetComponent<UILabel>();

        mModelTexture = transform.Find("ModelTexture").GetComponent<UITexture>();
        InitItems();
        ShowRender();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnReturn).onClick = OnReturnClick;
        UIEventListener.Get(btnUpgrade).onClick = OnUpgradeClick;
        UIEventListener.Get(btnBattle).onClick = OnBattleClick;
        UIEventListener.Get(btnUnload).onClick = OnUnloadClick;
        UIEventListener.Get(btnAutoToAddFood).onClick = OnAutoToAddFoodClick;
        UIEventListener.Get(mModelTexture.gameObject).onDrag = OnDragModel;
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
        PetService.Instance.TryUnloadPet(mCurPetID);
    }

    private void OnBattleClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        PetService.Instance.TryBattlePet(mCurPetID);
    }

    private void OnAutoToAddFoodClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        if (mItems.Count > 0)
        {
            return;
        }
        PetModule.Instance.GetItemListToOneKeyUpgrade(ref mItems);
        if (mItems.Count==0)
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
        PetService.Instance.TryUpgradePet(mCurPetID, mItems);
    }

    private void OnReturnClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_UPGRADE_PET, OnRecvUpgrade);
        GTEventCenter.AddHandler(GTEventID.TYPE_BATTLE_PET, OnRecvBattlePet);
        GTEventCenter.AddHandler(GTEventID.TYPE_UNLOAD_PET, OnRecvUnLoadPet);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_UPGRADE_PET, OnRecvUpgrade);
        GTEventCenter.DelHandler(GTEventID.TYPE_BATTLE_PET, OnRecvBattlePet);
        GTEventCenter.DelHandler(GTEventID.TYPE_UNLOAD_PET, OnRecvUnLoadPet);
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
            GameObject go = NGUITools.AddChild(leftGrid.gameObject, leftTemp);
            leftGrid.AddChild(go.transform);
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
                    leftCenterOnChild.enabled = true;
                    leftCenterOnChild.CenterOn(go.transform);
                    leftCenterOnChild.onCenter = delegate (GameObject centerObj)
                    {
                        leftCenterOnChild.enabled = false;
                    };
                }
                Switch(id);
            };

        }
        em.Dispose();
        leftGrid.repositionNow = true;
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
            mRender = ERender.AddRender(mModelTexture);
        }
        mAvatar = CharacterManager.Instance.DelAvatar(mAvatar);
        mRender.DelAllModels();
        mAvatar = CharacterManager.Instance.AddAvatar(db.Model);
        mAvatar.PlayAnim("idle", null);
        GameObject model = mRender.AddModel(mAvatar.GetRootObj());
        model.transform.localPosition = db.StagePos;
        model.transform.localEulerAngles = new Vector3(0, 120, 0);
        model.transform.localScale = new Vector3(db.StageScale, db.StageScale, db.StageScale);
    }

    private void ShowListView()
    {
        XCharacter role = RoleModule.Instance.GetCurPlayer();
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
        data = data == null ? DataFactory.CreatePet(mCurPetID) : data;
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
        GTItemHelper.ShowQualityText(title, str, db.Quality);


        ShowPropertyView(mCurPetID, data.Level);
        XCharacter role = RoleModule.Instance.GetCurPlayer();

        btnBattle.SetActive(role.PetID != mCurPetID);
        btnUnload.SetActive(role.PetID == mCurPetID);
    }

    private void ShowPropertyView(int id, int level)
    {
        DActor db = ReadCfgActor.GetDataById(id);
        currPropertyText1.text = string.Empty;
        currPropertyText2.text = string.Empty;
        mainPropertyText1.text = string.Empty;
        mainPropertyText2.text = string.Empty;

        DPetLevel levelDB = ReadCfgPetLevel.GetDataById(db.Quality * 1000 + level);
        for (int i = 0; i < levelDB.PropertyNums.Length; i++)
        {
            EAttr propertyID = (EAttr)(i + 1);
            DProperty propertyDB = ReadCfgProperty.GetDataById(propertyID);
            currPropertyText1.Append(propertyDB.Name);
            currPropertyText2.Append("+" + levelDB.PropertyNums[i].ToString());

            mainPropertyText1.Append(propertyDB.Name);
            int extendNum = (int)(levelDB.PropertyNums[i] * levelDB.Ratio / 100f);
            mainPropertyText2.Append("+" + extendNum.ToString());
        }

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
}
