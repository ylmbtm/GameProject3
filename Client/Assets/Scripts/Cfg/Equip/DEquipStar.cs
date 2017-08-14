using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DEquipStar : DObj<int>
{
    public int Id;
    public int Quality;
    public int StarLevel;
    public int Percent;
    public int CostItemId;
    public int CostMoneyId;
    public int CostMoneyNum;
    public int CostItemNum;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id           = element.GetInt32("Id");
        this.Quality      = element.GetInt32("Quality");
        this.StarLevel    = element.GetInt32("StarLevel");
        this.Percent      = element.GetInt32("Percent");
        this.CostItemId   = element.GetInt32("CostItemId");
        this.CostMoneyId  = element.GetInt32("CostMoneyId");
        this.CostItemNum  = element.GetInt32("CostItemNum");
        this.CostMoneyNum = element.GetInt32("CostMoneyNum");
    }
}

public class ReadCfgEquipStar : DReadBase<int, DEquipStar>
{

}