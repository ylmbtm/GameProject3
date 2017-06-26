using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DStore : DObj<int>
{
    public int Id;
    public int ItemID;
    public int ItemNum;
    public int CostMoneyID;
    public int CostMoneyNum;
    public int StoreType;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id           = element.GetInt("Id");
        this.ItemID       = element.GetInt("ItemID");
        this.ItemNum      = element.GetInt("ItemNum");
        this.CostMoneyNum = element.GetInt("CostMoneyNum");
        this.CostMoneyID  = element.GetInt("CostMoneyID");
        this.StoreType    = element.GetInt("StoreType");
    }
}

public class ReadCfgStore : DReadBase<int, DStore>
{

}