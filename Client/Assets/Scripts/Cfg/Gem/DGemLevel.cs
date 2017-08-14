using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DGemLevel : DObj<int>
{
    public const int GEM_STRENGTHEN_ITEM_ID_1 = 1031;
    public const int GEM_STRENGTHEN_ITEM_ID_2 = 1035;
    public const int GEM_STRENGTHEN_ITEM_ID_3 = 1033;
    public const int GEM_STRENGTHEN_ITEM_ID_4 = 1034;
    public const int GEM_STRENGTHEN_ITEM_ID_5 = 1035;

    public int Id;
    public int Quality;
    public int Level;
    public int CostMoneyId;
    public int RequireExp;
    public int PropertyRatio;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id            = element.GetInt32("Id");
        this.Quality       = element.GetInt32("Quality");
        this.Level         = element.GetInt32("Level");
        this.CostMoneyId   = element.GetInt32("UpCostMoneyId");
        this.RequireExp    = element.GetInt32("RequireExp");
        this.PropertyRatio = element.GetInt32("PropertyRatio");
    }
}

public class ReadCfgGemLevel : DReadBase<int, DGemLevel>
{

}