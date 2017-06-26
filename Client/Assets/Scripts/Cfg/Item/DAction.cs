using UnityEngine;
using System.Collections;
using System;
using System.Xml;

public class DAction : DObj<EAction>
{
    public EAction  Id;
    public int      ItemID;

    public override EAction GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id     = (EAction)element.GetInt("Id");
        this.ItemID = element.GetInt("ItemID");
    }
}

public class ReadCfgAction : DReadBase<EAction, DAction>
{

}