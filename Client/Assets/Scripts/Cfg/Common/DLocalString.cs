using UnityEngine;
using System.Collections;
using System;
using System.Xml;

public class DLocalString : DObj<string>
{
    public string Id;
    public string Value;

    public override string GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id    = element.GetString("Id");
        this.Value = element.GetString("Value");
    }
}

public class ReadCfgLocalString : DReadBase<string, DLocalString>
{

}