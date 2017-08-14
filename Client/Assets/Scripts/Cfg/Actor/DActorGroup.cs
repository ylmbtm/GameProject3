using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DActorGroup : DObj<int>
{
    public int           Id;
    public string        Name = string.Empty;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id   = element.GetInt32("Id");
        this.Name = element.GetString("Name");
    }
}

public class ReadCfgActorGroup : DReadBase<int, DActorGroup>
{

}