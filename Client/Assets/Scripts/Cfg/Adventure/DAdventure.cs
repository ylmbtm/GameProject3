using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DAdventure : DObj<int>
{
    public int    Id;
    public string Name = string.Empty;
    public string Icon = string.Empty;
    public int    Times;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id    = element.GetInt32("Id");
        this.Name  = element.GetString("Name");
        this.Icon  = element.GetString("Icon");
        this.Times = element.GetInt32("Times");
    }
}

public class ReadCfgAdventure : DReadBase<int, DAdventure>
{

}
