using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using Protocol;

public class UIPartnerStrength : GTWindow
{
    private Int32      mSelectID;
    private ERender    mRender;
    private UITexture  mModelTexture;
    private GameObject mModel;
    private GameObject btnReturn;

    private UIToggle   menuProperty;
    private UIToggle   menuSkill;
    private UIToggle   menuUpStar;
    private UIToggle   menuAwake;
    private UIToggle   menuWash;
    private GameObject btnUpLevel;

    private UILabel  partnerLevel;
    private UILabel  partnerExp;
    private UILabel  partnerName;
    private UILabel  partnerFightValue;
    private UILabel  partnerWakeLevel;
    private UISlider partnerExpValue;


    public UIPartnerStrength()
    {
        Type = EWindowType.WINDOW;
        mResident = false;
        mResPath = "Partner/UIPartnerStrength";
    }

    protected override void OnAwake()
    {
        mModelTexture = transform.Find("Left/Texture").GetComponent<UITexture>();
        btnReturn = transform.Find("Top/Btn_Return").gameObject;

        Transform menuTrans = transform.Find("Left/Bottom/Menus");
        menuProperty = menuTrans.Find("Tab_Property").GetComponent<UIToggle>();
        menuSkill = menuTrans.Find("Tab_Skill").GetComponent<UIToggle>();
        menuUpStar = menuTrans.Find("Tab_UpStar").GetComponent<UIToggle>();
        menuAwake = menuTrans.Find("Tab_Awake").GetComponent<UIToggle>();
        menuWash= menuTrans.Find("Tab_Wash").GetComponent<UIToggle>();

        Transform levelTrans = transform.Find("Left/Bottom/Level");
        btnUpLevel = levelTrans.Find("Btn_UpLevel").gameObject;

        partnerExp = levelTrans.Find("Exp/Value").GetComponent<UILabel>();
        partnerExpValue = levelTrans.Find("Exp").GetComponent<UISlider>();
        partnerName = levelTrans.Find("Name").GetComponent<UILabel>();
        partnerFightValue=levelTrans.Find("FightValue").GetComponent<UILabel>();
        partnerWakeLevel = levelTrans.Find("WakeLevel").GetComponent<UILabel>();
        partnerLevel = levelTrans.Find("Level").GetComponent<UILabel>();
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnReturn).onClick = OnCloseClick;
        UIEventListener.Get(mModelTexture.gameObject).onDrag = OnTextureDrag;
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
        XPartner data = DataDBSPartner.GetDataById(mSelectID);
        if(data==null)
        {
            data = DataFactory.CreatePartner(mSelectID);
        }
        partnerLevel.text = GTTools.Format("等级 {0}", data.Level);
        partnerExp.text = GTTools.Format("{0}/{1}", data.Exp, 2500);
        partnerExpValue.value = 0;
        partnerFightValue.text = GTTools.Format("战斗力：{0}", AttrHelper.GetFightValue(data));
        partnerWakeLevel.text = GTTools.Format("觉醒 {0}", data.Wake);
        DActor db = ReadCfgActor.GetDataById(mSelectID);
        GTItemHelper.ShowQualityText(partnerName, db.Name, db.Quality);
    }

    private void OnUpLevelClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnCloseClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }

    private void OnTextureDrag(GameObject go, Vector2 delta)
    {
        if (mModel == null)
        {
            return;
        }
        ESpin.Get(mModel).OnSpin(delta, 2);
    }

    private void InitModel()
    {
        mRender = ERender.AddRender(mModelTexture);
    }

    public void SetID(int id)
    {
        this.mSelectID = id;
        this.InitModel();
        this.ShowView();
    }
}
