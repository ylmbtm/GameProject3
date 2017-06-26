using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DEquipStreng : DObj<int>
{
    public int                    Id;
    public List<KStrengthValue>   Propertys = new List<KStrengthValue>();

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id = element.GetInt("Id");
        for (int i = 1; i <= 6; i++)
        {
            KStrengthValue data = new KStrengthValue();
            data.Property    = (EAttr)element.GetInt("PropertyId" + i);
            data.Value       = element.GetInt("PropertyNum" + i);
            data.UnlockLevel = element.GetInt("PropertyLevel" + i);
            this.Propertys.Add(data);
        }
    }
}


public class ReadCfgEquipStreng : DReadBase<int, DEquipStreng>
{

}