using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DEquipAdvanceCost : DObj<int>
{
    public int Id;
    public int Quality;
    public int AdvanceLevel;
    public int CostItemId;
    public int CostItemNum;
    public int CostMoneyNum;
    public int CostMoneyId;
    public int CostEquipNum;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id              = element.GetInt("Id");
        this.Quality         = element.GetInt("Quality");
        this.AdvanceLevel    = element.GetInt("AdvanceLevel");
        this.CostItemId      = element.GetInt("CostItemId");
        this.CostItemNum     = element.GetInt("CostItemNum");
        this.CostMoneyId     = element.GetInt("CostMoneyId");
        this.CostMoneyNum    = element.GetInt("CostMoneyNum");
        this.CostEquipNum    = element.GetInt("CostEquipNum");
    }
}

public class ReadCfgEquipAdvanceCost : DReadBase<int, DEquipAdvanceCost>
{

}