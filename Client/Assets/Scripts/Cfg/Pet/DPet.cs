using UnityEngine;
using System.Collections;
using System;
using System.Xml;

public class DPet : DObj<int>
{
    public int Id;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id = element.GetInt32("Id");
    }
}

public class ReadCfgPet : DReadBase<int, DPet>
{

}