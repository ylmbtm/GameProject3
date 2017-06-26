using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

public enum ETaskType
{
    NONE,
    THREAD=1,   //主线任务
    BRANCH=2,   //支线任务
    DAILY =3,   //日常任务
}

public enum ETaskTargetType
{
   TYPE_NONE,
   TYPE_KILL_COPYBOSS         =1,//击杀副本BOSS
   TYPE_MAIN_PASSCOPY         =2,//通关副本（次数）
   TYPE_UPEQUIP               =3,//升级装备
   TYPE_UPPET                 =4,//升级宠物
   TYPE_UPGEM                 =5,//升级星石
   TYPE_UPPARTNER             =6,//升级伙伴
   TYPE_UPSKILL               =7,//升级角色技能
   TYPE_TALK                  =8,//对话
   TYPE_ROB_TREASURE          =9,//夺宝
   TYPE_AREAE                 =10,//竞技场战斗
   TYPE_PASS_ELITECOPY        =11,//通关精英副本
   TYPE_CHARGE_RELICE         =12,//神器充能
   TYPE_EQUIPSTAR             =13,//装备星级
   TYPE_XHJJC                 =14,//虚幻竞技场
   TYPE_KILLRACE              =15,//杀死种族怪物
}


public enum ETaskCycleType
{
    TYPE_NONE,    
    TYPE_DAILY,   //每日重置
    TYPE_WEEKLY,  //每周重置
    TYPE_SCENE,   //副本重置
}

public enum ETaskState
{
    QUEST_NONE,           //无类型
    QUEST_DOING,          //正在进行任务
    QUEST_CANSUBMIT,      //可提交
    QUEST_FAILED,         //任务失败
    QUEST_HASSUBMIT,      //已经提交
}

public class DTask : DObj<int>
{
    public int              Id;
    public string           Name = string.Empty;
    public string           Desc = string.Empty;
    public string           Icon = string.Empty;
    public ETaskType        Type;
    public ETaskCycleType   Cycle;
    public ETaskTargetType  TargetType;
    public string           TargetArgs = string.Empty;
    public int              MinRquireLevel;
    public int              MaxRquireLevel;
    public int              Condition;
    public int              RewardMoneyNum;
    public int              RewardExpNum;
    public int              AwardID;
    public string           Script;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id             = element.GetInt("Id");
        this.Name           = element.GetString("Name");
        this.Desc           = element.GetString("Desc");
        this.Type           = (ETaskType)element.GetInt("Type");
        this.Cycle          = (ETaskCycleType)element.GetInt("Cycle");
        this.TargetType     = (ETaskTargetType)element.GetInt("TargetType");
        this.TargetArgs     = element.GetString("TargetArgs");
        this.Script         = element.GetString("Script");
        this.Condition      = element.GetInt("Condition");
        this.MinRquireLevel = element.GetInt("MinRquireLevel");
        this.MaxRquireLevel = element.GetInt("MaxRquireLevel");
        this.RewardMoneyNum = element.GetInt("RewardMoneyNum");
        this.RewardExpNum   = element.GetInt("RewardExpNum");
        this.AwardID        = element.GetInt("AwardID");
        this.Desc           = element.GetString("Desc");
        this.Icon           = element.GetString("Icon");
    }

    public const int TASK_THREAD = 1;
    public const int TASK_BRANCH = 2;
    public const int TASK_DAILY  = 3;
}

public class ReadCfgTask : DReadBase<int, DTask>
{

}