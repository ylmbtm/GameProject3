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
        this.Id           = element.GetInt("Id");
        this.Quality      = element.GetInt("Quality");
        this.StarLevel    = element.GetInt("StarLevel");
        this.Percent      = element.GetInt("Percent");
        this.CostItemId   = element.GetInt("CostItemId");
        this.CostMoneyId  = element.GetInt("CostMoneyId");
        this.CostItemNum  = element.GetInt("CostItemNum");
        this.CostMoneyNum = element.GetInt("CostMoneyNum");
    }
}

public class ReadCfgEquipStar : DReadBase<int, DEquipStar>
{

}