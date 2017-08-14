using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DGem : DObj<int>
{
    public int                    Id;
    public string                 Name;
    public int                    Quality;
    public List<DStrengthValue>   Propertys = new List<DStrengthValue>();
    public int                    Suit;
    public int                    Pos;
    public int                    Exp;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id      = element.GetInt32("Id");
        this.Name    = element.GetString("Name");
        this.Quality = element.GetInt32("Quality");
        this.Suit    = element.GetInt32("Suit");
        this.Exp     = element.GetInt32("Exp");
        this.Pos     = element.GetInt32("Pos");

        for (int i = 1; i <= 3; i++)
        {
            EAttr k = (EAttr)element.GetInt32("PropertyId" + i);
            int v = element.GetInt32("PropertyNum" + i);
            int l = element.GetInt32("UnlockLevel" + i);
            DStrengthValue gem = new DStrengthValue(k, v, l);
            this.Propertys.Add(gem);
        }
    }
}

public class ReadCfgGem : DReadBase<int, DGem>
{

}