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
    public List<KStrengthValue>   Propertys = new List<KStrengthValue>();
    public int                    Suit;
    public int                    Pos;
    public int                    Exp;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id      = element.GetInt("Id");
        this.Name    = element.GetString("Name");
        this.Quality = element.GetInt("Quality");
        this.Suit    = element.GetInt("Suit");
        this.Exp     = element.GetInt("Exp");
        this.Pos     = element.GetInt("Pos");

        for (int i = 1; i <= 3; i++)
        {
            EAttr k = (EAttr)element.GetInt("PropertyId" + i);
            int v = element.GetInt("PropertyNum" + i);
            int l = element.GetInt("UnlockLevel" + i);
            KStrengthValue gem = new KStrengthValue(k, v, l);
            this.Propertys.Add(gem);
        }
    }
}

public class ReadCfgGem : DReadBase<int, DGem>
{

}