using Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class UIHeroInfo : GTWindow
{
    private UILabel heroLevel;
    private UILabel heroName;
    private UILabel heroVIP;
    private UILabel heroExpNum;
    private UILabel heroFightValue;
    private UILabel labActionNum1;
    private UILabel labActionName1;
    private UILabel labOneTimer1;
    private UILabel labAllTimer1;
    private UILabel labActionNum2;
    private UILabel labActionName2;
    private UILabel labOneTimer2;
    private UILabel labAllTimer2;
    private List<MoneyView> mMoneyViewList = new List<MoneyView>();
    private GameObject btnClose;
    private GameObject btnChangeName;
    private UISlider heroExpBar;

    class MoneyView
    {
        public UITexture texture;
        public UILabel labNum;
    }

    public UIHeroInfo()
    {
        Type = EWindowType.DIALOG;
        mResident = false;
        mResPath = "Home/UIHeroInfo";
    }

    protected override void OnAwake()
    {
        heroName = transform.Find("Name").GetComponent<UILabel>();
        heroLevel = transform.Find("Level").GetComponent<UILabel>();
        heroVIP = transform.Find("VIPLevel").GetComponent<UILabel>();
        heroFightValue = transform.Find("FightValue").GetComponent<UILabel>();
        heroExpBar = transform.Find("ExpBar").GetComponent<UISlider>();
        heroExpNum = transform.Find("ExpBar/Num").GetComponent<UILabel>();

        labActionNum1 =transform.Find("Actions/1/Num").GetComponent<UILabel>();
        labActionName1=transform.Find("Actions/1/ActionName").GetComponent<UILabel>();
        labAllTimer1=transform.Find("Actions/1/AllTimer").GetComponent<UILabel>();
        labOneTimer1=transform.Find("Actions/1/OneTimer").GetComponent<UILabel>();
        labActionNum2 = transform.Find("Actions/2/Num").GetComponent<UILabel>();
        labActionName2 = transform.Find("Actions/2/ActionName").GetComponent<UILabel>();
        labAllTimer2 = transform.Find("Actions/2/AllTimer").GetComponent<UILabel>();
        labOneTimer2 = transform.Find("Actions/2/OneTimer").GetComponent<UILabel>();

        btnClose = transform.Find("Btn_Close").gameObject;
        btnChangeName = transform.Find("Btn_ChangeName").gameObject;

        Transform moneysTrans = transform.Find("Moneys");
        for(int i=1;i<=12;i++)
        {
            MoneyView mv = new MoneyView();
            Transform money = moneysTrans.Find(i.ToString());
            mv.texture = money.Find("Texture").GetComponent<UITexture>();
            mv.labNum=money.Find("Num").GetComponent<UILabel>();
            mMoneyViewList.Add(mv);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloceClick;
        UIEventListener.Get(btnChangeName).onClick = OnChangeNameClick;
    }


    protected override void OnAddHandler()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_SECOND_TICK, ShowActionView);
        GTEventCenter.AddHandler(GTEventID.TYPE_CHANGE_HEROEXP, ShowView);
        GTEventCenter.AddHandler(GTEventID.TYPE_CHANGE_HEROLEVEL, ShowView);
        GTEventCenter.AddHandler(GTEventID.TYPE_CHANGE_MONEY, ShowView);
    }

    protected override void OnDelHandler()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_SECOND_TICK, ShowActionView);
        GTEventCenter.DelHandler(GTEventID.TYPE_CHANGE_HEROEXP, ShowView);
        GTEventCenter.DelHandler(GTEventID.TYPE_CHANGE_HEROLEVEL, ShowView);
        GTEventCenter.DelHandler(GTEventID.TYPE_CHANGE_MONEY, ShowView);
    }

    protected override void OnEnable()
    {
        ShowView();
        ShowActionView();
    }

    protected override void OnClose()
    {
        mMoneyViewList.Clear();
    }

    private void ShowView()
    {
        for (int i=0;i<mMoneyViewList.Count;i++)
        {
            MoneyView mv = mMoneyViewList[i];
            DItem db = ReadCfgItem.GetDataById(i + 1);
            GTItemHelper.ShowItemTexture(mv.texture, db.Id);
            mv.labNum.text = DataManager.Instance.GetItemCountById(db.Id).ToString();
        }

        XCharacter role = RoleModule.Instance.GetCurPlayer();
        DRoleLevel heroLevelDB = ReadCfgRoleLevel.GetDataById(role.Level);
        heroName.text = role.Name;
        heroLevel.text = GTTools.Format("等级 {0}", role.Level);
        heroVIP.text = GTTools.Format("VIP {0}", role.VipLevel);
        heroExpNum.text = GTTools.Format("{0}/{1}", role.CurExp, heroLevelDB.RequireExp);
        heroExpBar.value = role.CurExp / (heroLevelDB.RequireExp * 1f);

        int fightValue = AttrHelper.GetFightValue(role);
        heroFightValue.text = GTTools.Format("战斗力 {0}", fightValue);
    }

    private void ShowActionView()
    {
        DItem db1 = ReadCfgItem.GetDataById(101);
        DItem db2 = ReadCfgItem.GetDataById(102);
        labActionName1.text = db1.Name;
        labActionName2.text = db2.Name;

        int curNum1 = DataManager.Instance.GetActionCountByType(EAction.Manual);
        int curNum2 = DataManager.Instance.GetActionCountByType(EAction.Energy);
        labActionNum1.text = GTTools.Format("{0}/{1}",curNum1, db1.Data1);
        labActionNum2.text = GTTools.Format("{0}/{1}",curNum2, db2.Data1);

        string oneTimer1 = DataTimer.Instance.GetOneSecondTimer(EAction.Manual);
        string oneTimer2 = DataTimer.Instance.GetOneSecondTimer(EAction.Energy);
        string allTimer1 = DataTimer.Instance.GetAllSecondTimer(EAction.Manual);
        string allTimer2 = DataTimer.Instance.GetAllSecondTimer(EAction.Energy);

        labOneTimer1.text = GTTools.Format("{0}恢复一点:{1}", db1.Name, oneTimer1);
        labOneTimer2.text = GTTools.Format("{0}恢复一点:{1}", db2.Name, oneTimer2);
        labAllTimer1.text = GTTools.Format("{0}完全恢复:{1}", db1.Name, allTimer1);
        labAllTimer2.text = GTTools.Format("{0}完全恢复:{1}", db2.Name, allTimer2);
    }

    private void OnChangeNameClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnCloceClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
        Hide();
    }
}

