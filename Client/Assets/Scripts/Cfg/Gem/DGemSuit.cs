using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

public class DGemSuit : DObj<int>
{
    public int                              Id;
    public string                           Name;
    public List<Dictionary<EAttr, int>>     SuitPropertys = new List<Dictionary<EAttr, int>>();
    public string                           SuitDesc;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id       = element.GetInt32("Id");
        this.Name     = element.GetString("Name");
        this.SuitDesc = element.GetString("SuitDesc");
        for (int i = 1; i <= 3; i++)
        {
            string[] suit = element.GetString("Suit" + i).Split('|');
            Dictionary<EAttr, int> propertys = new Dictionary<EAttr, int>();
            propertys.Add(EAttr.HP, suit[0].ToInt32());
            propertys.Add(EAttr.AP, suit[1].ToInt32());
            this.SuitPropertys.Add(propertys);
        }
    }
}


public class ReadCfgGemSuit : DReadBase<int, DGemSuit>
{

}