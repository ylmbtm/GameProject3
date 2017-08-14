using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DEffect : DObj<int>
{
    public int          Id;
    public string       Path = string.Empty;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id   = element.GetInt32("Id");
        this.Path = element.GetString("Path");
    }
}

public class ReadCfgEffect : DReadBase<int, DEffect>
{

}