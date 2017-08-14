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
        this.Id          = element.GetInt32("Id");
        this.Level       = element.GetInt32("Level");
        this.CostSoulNum = element.GetInt32("CostSoulNum");
        this.Desc        = element.GetString("Desc");
    }
}

public class ReadCfgPartnerWake : DReadBase<int, DPartnerWake>
{

}