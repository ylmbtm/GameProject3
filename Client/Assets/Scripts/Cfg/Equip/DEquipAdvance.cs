using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DEquipAdvance : DObj<int>
{
    public int                                Id;
    public string                             Name;
    public int                                Quality;
    public List<KeyValuePair<EAttr, int>>     Propertys = new List<KeyValuePair<EAttr, int>>();

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id        = element.GetInt32("Id");
        this.Name      = element.GetString("Name");
        this.Quality   = element.GetInt32("Quality");
        for (int i = 1; i <= 8; i++)
        {
            EAttr key = (EAttr)element.GetInt32("PropertyId" + i);
            int value = element.GetInt32("PropertyNum" + i);
            KeyValuePair<EAttr, int> e = new KeyValuePair<EAttr, int>(key, value);
            this.Propertys.Add(e);
        }
    }
}

public class ReadCfgEquipAdvance : DReadBase<int, DEquipAdvance>
{

}