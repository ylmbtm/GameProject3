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
        this.Id     = (EAction)element.GetInt32("Id");
        this.ItemID = element.GetInt32("ItemID");
    }
}

public class ReadCfgAction : DReadBase<EAction, DAction>
{

}