using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DStoreType : DObj<int>
{
    public int         Id;
    public string      Name;
    public bool        Random;
    public string      Icon1;
    public string      Icon2;
    public int         Limit;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id     = element.GetInt32("Id");
        this.Name   = element.GetString("Name");
        this.Icon1  = element.GetString("Icon1");
        this.Icon2  = element.GetString("Icon2");
        this.Random = element.GetBool("Random");
        this.Limit  = element.GetInt32("Limit");
    }
}

public class ReadCfgStoreType : DReadBase<int, DStoreType>
{ 

}