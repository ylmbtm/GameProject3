using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIPartnerStrength : GTWindow
{
    private Int32           selectID;
    private UITexture       modelTexture;
    private GameObject      model;
    private GameObject      btnClose;

    private UIToggle        menuProperty;
    private UIToggle        menuSkill;
    private UIToggle        menuUpStar;
    private UIToggle        menuAwake;
    private UIToggle        menuWash;
    private GameObject      btnUpLevel;

    private UILabel         partnerLevel;
    private UILabel         partnerExp;
    private UILabel         partnerName;
    private UISlider        partnerExpValue;

    private ERender         mRender;
    private CharacterAvatar mAvatar;

    public UIPartnerStrength()
    {
        Type = EWindowType.Window;
        Resident = false;
        ShowMode = EWindowShowMode.HideOther;
        MaskType = EWindowMaskType.Black;
        Path = "Partner/UIPartnerStrength";
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        modelTexture = pivot.Find("Texture").GetComponent<UITexture>();
        btnClose = pivot.Find("BtnClose").gameObject;

        Transform menuTrans = pivot.Find("Menus");
        menuProperty = menuTrans.Find("Tab_Property").GetComponent<UIToggle>();
        menuSkill = menuTrans.Find("Tab_Skill").GetComponent<UIToggle>();
        menuUpStar = menuTrans.Find("Tab_UpStar").GetComponent<UIToggle>();
        menuAwake = menuTrans.Find("Tab_Awake").GetComponent<UIToggle>();
        menuWash= menuTrans.Find("Tab_Wash").GetComponent<UIToggle>();

        Transform levelTrans = pivot.Find("Level");
        btnUpLevel = levelTrans.Find("Btn_UpLevel").gameObject;
        partnerExp = levelTrans.Find("Exp/Value").GetComponent<UILabel>();
        partnerExpValue = levelTrans.Find("Exp").GetComponent<UISlider>();
        partnerName = levelTrans.Find("Name").GetComponent<UILabel>();
        partnerLevel = levelTrans.Find("Level").GetComponent<UILabel>();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloseClick;
        UIEventListener.Get(modelTexture.gameObject).onDrag = OnTextureDrag;
        UIEventListener.Get(btnUpLevel).onClick = OnUpLevelClick;
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
        ERender.DelRender(mRender);
        mRender = null;
    }

    private void ShowView()
    {
        XPartner data = DataDBSPartner.GetDataById(selectID);
        if (data == null)
        {
            data = GTDataFactory.CreatePartner(selectID);
        }
        partnerLevel.text = GTTools.Format("等级 {0}", data.Level);
        partnerExp.text = GTTools.Format("{0}/{1}", data.Exp, 2500);
        partnerExpValue.value = 0;
        DActor db = ReadCfgActor.GetDataById(selectID);
        GTItemHelper.ShowQualityText(partnerName, db.Name, db.Quality);
    }

    private void OnUpLevelClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }

    private void OnTextureDrag(GameObject go, Vector2 delta)
    {
        if (model == null)
        {
            return;
        }
        ESpin.Get(model).OnSpin(delta, 2);
    }

    private void InitModel()
    {
        DActor db = ReadCfgActor.GetDataById(selectID);
        mRender = ERender.AddRender(modelTexture);
        mRender.DelAllModels();
        mAvatar = GTWorld.Instance.AddAvatar(db.Model);
        mAvatar.PlayAnim("idle", null);
        GameObject model = mRender.AddModel(mAvatar.GetRootObj());
        DStage ds = ReadCfgStagePartner.GetDataById(selectID);
        model.transform.localPosition = new Vector3(ds.X, ds.Y, ds.Z);
        model.transform.localEulerAngles = new Vector3(0, 180, 0);
        model.transform.localScale = Vector3.one * ds.Scale * 0.3f;
    }

    public void SetID(int id)
    {
        this.selectID = id;
        this.InitModel();
        this.ShowView();
    }
}
