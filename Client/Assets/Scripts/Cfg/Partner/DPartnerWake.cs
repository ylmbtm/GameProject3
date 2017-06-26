using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DPartnerWake : DObj<int>
{
    public int    Id;
    public int    Level;
    public string Desc = string.Empty;
    public int    CostSoulNum;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id          = element.GetInt("Id");
        this.Level       = element.GetInt("Level");
        this.CostSoulNum = element.GetInt("CostSoulNum");
        this.Desc        = element.GetString("Desc");
    }
}

public class ReadCfgPartnerWake : DReadBase<int, DPartnerWake>
{

}