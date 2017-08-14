using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

public class DEquip : DObj<int>
{
    public int                         Id;
    public string                      Name;
    public int                         Quality;
    public int                         Pos;
    public Dictionary<EAttr, int>      Propertys = new Dictionary<EAttr, int>();
    public int                         StrengthGrow1;
    public int                         StrengthGrow2;
    public int                         StrengthGrow3;
    public int                         DeComposeNum1;
    public int                         DeComposeNum2;
    public int                         DeComposeId1;
    public int                         DeComposeId2;
    public int                         Suit;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id            = element.GetInt32("Id");
        this.Name          = element.GetString("Name");
        this.Quality       = element.GetInt32("Quality");
        this.Pos = element.GetInt32("Pos");
        for (int i = 1; i <= 10; i++)
        {
            int p = element.GetInt32("P" + i);
            this.Propertys.Add((EAttr)i, p);
        }
        this.Suit = element.GetInt32("Suit");
        this.StrengthGrow1 = element.GetInt32("StrengthenGrow1");
        this.StrengthGrow2 = element.GetInt32("StrengthenGrow2");
        this.StrengthGrow3 = element.GetInt32("StrengthenGrow3");
        this.DeComposeId1  = element.GetInt32("DeComposeId1");
        this.DeComposeId2  = element.GetInt32("DeComposeId2");
        this.DeComposeNum1 = element.GetInt32("DeComposeNum1");
        this.DeComposeNum2 = element.GetInt32("DeComposeNum2");
    }

    public const int EQUIP_STRENGTHEN_MONEY_ID_1 = 1;
    public const int EQUIP_STRENGTHEN_MONEY_ID_2 = 3;
}

public class ReadCfgEquip : DReadBase<int, DEquip>
{

}