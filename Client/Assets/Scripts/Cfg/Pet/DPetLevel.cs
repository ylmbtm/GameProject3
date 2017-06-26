using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DPetLevel : DObj<int>
{
    public int    Id;
    public int    Quality;
    public int    Level;
    public int    Exp;
    public int[]  PropertyNums = new int[3];
    public int    Ratio;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id      = element.GetInt("Id");
        this.Quality = element.GetInt("Quality");
        this.Level   = element.GetInt("Level");
        this.Exp     = element.GetInt("Exp");
        this.Ratio   = element.GetInt("Ratio");
        for (int i = 1; i <= 3; i++)
        {
            this.PropertyNums[i - 1] = element.GetInt("PropertyNum" + i);
        }
    }

    public const int PET_UP_ITEM_ID_1 = 1036;
    public const int PET_UP_ITEM_ID_2 = 1037;
    public const int PET_UP_ITEM_ID_3 = 1038;
}

public class ReadCfgPetLevel : DReadBase<int, DPetLevel>
{

}