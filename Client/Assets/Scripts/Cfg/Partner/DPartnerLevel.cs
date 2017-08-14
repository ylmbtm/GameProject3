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
        this.Id      = element.GetInt32("Id");
        this.Level   = element.GetInt32("Level");
        this.Partner = element.GetInt32("Partner");
        this.Exp     = element.GetInt32("Exp");
        for (int i = 1; i <= 3; i++)
        {
            this.Propertys[i - 1] = element.GetInt32("Property" + i);
        }
    }
}

public class ReadCfgPartnerLevel : DReadBase<int, DPartnerLevel>
{

}