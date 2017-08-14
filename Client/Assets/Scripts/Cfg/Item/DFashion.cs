using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DFashion : DObj<int>
{
    public int                                Id;
    public List<KeyValuePair<EAttr, int>>     Propertys = new List<KeyValuePair<EAttr, int>>();

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id = element.GetInt32("Id");
        for (int i = 1; i <= 2; i++)
        {
            EAttr e = (EAttr)element.GetInt32("PropertyId" + i);
            int v = element.GetInt32("PropertyNum" + i);
            KeyValuePair<EAttr, int> fp = new KeyValuePair<EAttr, int>(e, v);
            this.Propertys.Add(fp);
        }
    }
}

public class ReadCfgFashion : DReadBase<int, DFashion>
{

}
