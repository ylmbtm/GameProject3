using Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;

public class UIHeroInfo : GTWindow
{
    private UILabel         heroLevel;
    private UILabel         heroName;
    private UILabel         heroVIP;
    private UILabel         heroExpNum;
    private UILabel         heroFightValue;
    private UILabel         labActionNum1;
    private UILabel         labOneTimer1;
    private UILabel         labAllTimer1;
    private UILabel         labActionNum2;
    private UILabel         labOneTimer2;
    private UILabel         labAllTimer2;
    private List<MoneyView> moneyViewList = new List<MoneyView>();
    private GameObject      btnClose;
    private UISlider        heroExpBar;

    class MoneyView
    {
        public UITexture    moneyTexture;
        public UILabel      moneyNumText;
    }

    public UIHeroInfo()
    {
        Type = EWindowType.Window;
        Resident = false;
        Path = "Home/UIHeroInfo";
        MaskType = EWindowMaskType.Black;
        ShowMode = EWindowShowMode.HideOther;
    }

    protected override void OnAwake()
    {
        Transform pivot = transform.Find("Pivot");
        heroName = pivot.Find("HeroName").GetComponent<UILabel>();
        heroLevel = pivot.Find("HeroLevel").GetComponent<UILabel>();
        heroVIP = pivot.Find("HeroVIPLevel").GetComponent<UILabel>();
        heroFightValue = pivot.Find("HeroFightValue").GetComponent<UILabel>();
        heroExpBar = pivot.Find("HeroExpBar").GetComponent<UISlider>();
        heroExpNum = pivot.Find("HeroExpBar/Num").GetComponent<UILabel>();

        Transform heroActionTrans = pivot.Find("HeroActions");
        labActionNum1 = heroActionTrans.Find("1/Num").GetComponent<UILabel>();
        labAllTimer1 = heroActionTrans.Find("1/AllTimer").GetComponent<UILabel>();
        labOneTimer1 = heroActionTrans.Find("1/OneTimer").GetComponent<UILabel>();
        labActionNum2 = heroActionTrans.Find("2/Num").GetComponent<UILabel>();
        labAllTimer2 = heroActionTrans.Find("2/AllTimer").GetComponent<UILabel>();
        labOneTimer2 = heroActionTrans.Find("2/OneTimer").GetComponent<UILabel>();

        btnClose = pivot.Find("BtnClose").gameObject;
        Transform moneysTrans = pivot.Find("HeroMoneys");
        for (int i = 1; i <= 12; i++)
        {
            MoneyView mv = new MoneyView();
            Transform money = moneysTrans.Find(i.ToString());
            mv.moneyTexture = money.Find("Texture").GetComponent<UITexture>();
            mv.moneyNumText = money.Find("Num").GetComponent<UILabel>();
            moneyViewList.Add(mv);
        }
    }

    protected override void OnAddButtonListener()
    {
        UIEventListener.Get(btnClose).onClick = OnCloceClick;
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
        moneyViewList.Clear();
    }

    private void ShowView()
    {
        for (int i = 0; i < moneyViewList.Count; i++)
        {
            MoneyView mv = moneyViewList[i];
            DItem db = ReadCfgItem.GetDataById(i + 1);
            GTItemHelper.ShowItemTexture(mv.moneyTexture, db.Id);
            mv.moneyNumText.text = GTDataManager.Instance.GetItemCountById(db.Id).ToString();
        }

        XCharacter role = GTGlobal.Main;
        DRoleLevel heroLevelDB = ReadCfgRoleLevel.GetDataById(role.Level);
        heroName.text = role.Name;
        heroLevel.text = GTTools.Format("等级 {0}", role.Level);
        heroVIP.text = GTTools.Format("VIP {0}", role.VipLevel);
        heroExpNum.text = GTTools.Format("{0}/{1}", role.CurExp, heroLevelDB.RequireExp);
        heroExpBar.value = role.CurExp / (heroLevelDB.RequireExp * 1f);
        int fightValue = GTAttrHelper.GetFightValue(role);
        heroFightValue.text = GTTools.Format("战斗力 {0}", fightValue);
    }

    private void ShowActionView()
    {
        DItem db1 = ReadCfgItem.GetDataById(101);
        DItem db2 = ReadCfgItem.GetDataById(102);
        int curNum1 = GTDataManager.Instance.GetActionCountByType(EAction.Manual);
        int curNum2 = GTDataManager.Instance.GetActionCountByType(EAction.Energy);
        labActionNum1.text = GTTools.Format("{0} {1}/{2}", db1.Name, curNum1, db1.Data1);
        labActionNum2.text = GTTools.Format("{0} {1}/{2}", db1.Name, curNum2, db2.Data1);

        string oneTimer1 = GTDataTimer.Instance.GetOneSecondTimer(EAction.Manual);
        string oneTimer2 = GTDataTimer.Instance.GetOneSecondTimer(EAction.Energy);
        string allTimer1 = GTDataTimer.Instance.GetAllSecondTimer(EAction.Manual);
        string allTimer2 = GTDataTimer.Instance.GetAllSecondTimer(EAction.Energy);

        labOneTimer1.text = GTTools.Format("{0}恢复一点：{1}", db1.Name, oneTimer1);
        labOneTimer2.text = GTTools.Format("{0}恢复一点：{1}", db2.Name, oneTimer2);
        labAllTimer1.text = GTTools.Format("{0}完全恢复：{1}", db1.Name, allTimer1);
        labAllTimer2.text = GTTools.Format("{0}完全恢复：{1}", db2.Name, allTimer2);
    }

    private void OnChangeNameClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLICK);
    }

    private void OnCloceClick(GameObject go)
    {
        GTAudioManager.Instance.PlayEffectAudio(GTAudioKey.SOUND_UI_CLOSE);
        Hide();
    }
}

