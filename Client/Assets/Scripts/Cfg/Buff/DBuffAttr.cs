using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DBuffAttr : DObj<int>
{
    public int                Id;
    public string             Name = string.Empty;
    public List<KAttrValue>   Attrs = new List<KAttrValue>();

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id   = element.GetInt("Id");
        this.Name = element.GetString("Name");
        for (int i = 1; i <= 3; i++)
        {
            int attr = element.GetInt("Attr" + i);
            if (attr <= 0)
            {
                continue;
            }
            KAttrValue data = new KAttrValue();
            data.Attr      = (EAttr)attr;
            data.Value     = element.GetInt("Value" + i);
            data.ValueType = (EValueType)element.GetInt("ValueType" + i);
            this.Attrs.Add(data);
        }
    }
}

public class ReadCfgBuffAttr : DReadBase<int, DBuffAttr>
{

}