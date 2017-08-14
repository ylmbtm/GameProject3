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
        this.Id              = element.GetInt32("Id");
        this.Quality         = element.GetInt32("Quality");
        this.AdvanceLevel    = element.GetInt32("AdvanceLevel");
        this.CostItemId      = element.GetInt32("CostItemId");
        this.CostItemNum     = element.GetInt32("CostItemNum");
        this.CostMoneyId     = element.GetInt32("CostMoneyId");
        this.CostMoneyNum    = element.GetInt32("CostMoneyNum");
        this.CostEquipNum    = element.GetInt32("CostEquipNum");
    }
}

public class ReadCfgEquipAdvanceCost : DReadBase<int, DEquipAdvanceCost>
{

}