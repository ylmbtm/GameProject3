using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;


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
        this.Id             = element.GetInt32("Id");
        this.Name           = element.GetString("Name");
        this.Desc           = element.GetString("Desc");
        this.Type           = (ETaskType)element.GetInt32("Type");
        this.Cycle          = (ETaskCycleType)element.GetInt32("Cycle");
        this.TargetType     = (ETaskTargetType)element.GetInt32("TargetType");
        this.TargetArgs     = element.GetString("TargetArgs");
        this.Script         = element.GetString("Script");
        this.Condition      = element.GetInt32("Condition");
        this.MinRquireLevel = element.GetInt32("MinRquireLevel");
        this.MaxRquireLevel = element.GetInt32("MaxRquireLevel");
        this.RewardMoneyNum = element.GetInt32("RewardMoneyNum");
        this.RewardExpNum   = element.GetInt32("RewardExpNum");
        this.AwardID        = element.GetInt32("AwardID");
        this.Desc           = element.GetString("Desc");
        this.Icon           = element.GetString("Icon");
    }
}

public class ReadCfgTask : DReadBase<int, DTask>
{

}