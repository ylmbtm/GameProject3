using UnityEngine;
using System.Collections;
using System;
using System.Xml;

public class DRandomName : DObj<int>
{
    public int Key;
    public int Sex;
    public string Name;

    public override int GetKey()
    {
        return Key;
    }

    public override void Read(XmlElement element)
    {
        this.Key   = element.GetInt32("Key");
        this.Name  = element.GetString("Name");
        this.Sex   = element.GetInt32("Sex");
    }
}

public class ReadCfgRandomName : DReadBase<int, DRandomName>
{

}