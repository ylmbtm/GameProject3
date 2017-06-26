using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DPartnerFetter : DObj<int>
{
    public int      Id;
    public string   Name = string.Empty;
    public int[]    Targets = new int[2];
    public int      PropertyID;
    public int      PropertyNum;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id          = element.GetInt("Id");
        this.Name        = element.GetString("Name");
        this.PropertyID  = element.GetInt("PropertyID");
        this.PropertyNum = element.GetInt("PropertyNum");
        for (int i = 1; i <= 2; i++)
        {
            this.Targets[i - 1] = element.GetInt("Target" + i);
        }
    }
}

public class ReadCfgPartnerFetter : DReadBase<int, DPartnerFetter>
{

}