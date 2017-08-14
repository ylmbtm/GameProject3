using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DPartnerStar : DObj<int>
{
    public int     Id;
    public int[]   CostItemIDArray = new int[2];
    public int[]   CostItemNumArray = new int[2];
    public int     AddProperty;
    public int     RequireLevel;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id           = element.GetInt32("Id");
        this.AddProperty  = element.GetInt32("AddProperty");
        this.RequireLevel = element.GetInt32("RequireLevel");
        for (int i = 1; i <= 2; i++)
        {
            this.CostItemIDArray[i - 1] = element.GetInt32("CostItemID" + i);
            this.CostItemNumArray[i - 1] = element.GetInt32("CostItemNum" + i);
        }
    }
}

public class ReadCfgPartnerStar : DReadBase<int, DPartnerStar>
{

}