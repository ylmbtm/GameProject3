using UnityEngine;
using System.Collections;
using System;
using System.Xml;

public class DSkill : DObj<int>
{
    public int               Id;
    public string            Name      = string.Empty;
    public string            Icon      = string.Empty;
    public string            Desc      = string.Empty;
    public int               SkillType = 0;
    public int               Priority;
    public float             CountDown;
    public int               SkillCostType;
    public int               SkillCostNum;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id              = element.GetInt32("Id");
        this.Name            = element.GetString("Name");
        this.Icon            = element.GetString("Icon");
        this.Desc            = element.GetString("Desc");
        this.SkillType       = element.GetInt32("SkillType");
        this.Priority        = element.GetInt32("Priority");
        this.CountDown       = element.GetFloat("CountDown");
        this.SkillCostType   = element.GetInt32("SkillCostType");
        this.SkillCostNum    = element.GetInt32("SkillCostNum");
    }
}

public class ReadCfgSkill : DReadBase<int, DSkill>
{

}