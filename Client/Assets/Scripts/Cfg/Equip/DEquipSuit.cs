using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DEquipSuit : DObj<int>
{
    public int                              Id;
    public string                           Name          = string.Empty;
    public List<Dictionary<EAttr, int>>     SuitPropertys = new List<Dictionary<EAttr, int>>();

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id   = element.GetInt32("Id");
        this.Name = element.GetString("Name");
        for (int i = 1; i <= 3; i++)
        {
            string[] suit    = element.GetString("Suit" + i).Split(new char[1] { '|' }, StringSplitOptions.RemoveEmptyEntries);
            string[] idArray = element.GetString("SuitPropertyId" + i).Split(new char[1] { '|' }, StringSplitOptions.RemoveEmptyEntries);
            Dictionary<EAttr, int> d = new Dictionary<EAttr, int>();
            for (int j = 0; j < suit.Length; j++)
            {
                EAttr e = (EAttr)idArray[j].ToInt32();
                int v = suit[j].ToInt32();
                if (!d.ContainsKey(e))
                {
                    d.Add(e, v);
                }
            }
            this.SuitPropertys.Add(d);
        }
    }
}

public class ReadCfgEquipSuit : DReadBase<int, DEquipSuit>
{

}