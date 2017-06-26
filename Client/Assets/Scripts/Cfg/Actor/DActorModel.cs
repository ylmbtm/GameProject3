using UnityEngine;
using System.Collections;
using System;
using System.Xml;

public class DActorModel : DObj<int>
{
    public int    Id;
    public string Model = string.Empty;

    public override int  GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id    = element.GetInt("Id");
        this.Model = element.GetString("Model");
    }
}


public class ReadCfgActorModel : DReadBase<int, DActorModel>
{

}