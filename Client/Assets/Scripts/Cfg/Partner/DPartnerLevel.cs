using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DPartnerLevel : DObj<int>
{
    public const int MaxLevel = 60;

    public int   Id;
    public int   Partner;
    public int   Level;
    public int   Exp;
    public int[] Propertys = new int[3];

    public override int GetKey()
    {
        return Level;
    }

    public override void Read(XmlElement element)
    {
        this.Id      = element.GetInt("Id");
        this.Level   = element.GetInt("Level");
        this.Partner = element.GetInt("Partner");
        this.Exp     = element.GetInt("Exp");
        for (int i = 1; i <= 3; i++)
        {
            this.Propertys[i - 1] = element.GetInt("Property" + i);
        }
    }
}

public class ReadCfgPartnerLevel : DReadBase<int, DPartnerLevel>
{

}