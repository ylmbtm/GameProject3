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
        this.Id           = element.GetInt32("Id");
        this.ItemID       = element.GetInt32("ItemID");
        this.ItemNum      = element.GetInt32("ItemNum");
        this.CostMoneyNum = element.GetInt32("CostMoneyNum");
        this.CostMoneyID  = element.GetInt32("CostMoneyID");
        this.StoreType    = element.GetInt32("StoreType");
    }
}

public class ReadCfgStore : DReadBase<int, DStore>
{

}