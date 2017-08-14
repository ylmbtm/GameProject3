using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DActorRace : DObj<int>
{
    public int        Id;
    public EActorRace Race;
    public string     Name;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id   = element.GetInt32("Id");
        this.Race = (EActorRace)this.Id;
        this.Name = element.GetString("Name");
    }
}

public class ReadCfgActorRace : DReadBase<int, DActorRace>
{

}