using System;
using System.Collections.Generic;
using System.Xml;

public class DItem : DObj<int>
{
    public int              Id;
    public string           Name;
    public int              Quality;
    public EItemType        ItemType;
    public EBagType         BagType;
    public int              Carrer;
    public string           Model_R;
    public string           Model_L;
    public string           Icon;
    public int              SellMoneyId;
    public int              SellMoneyNum;
    public string           Desc;
    public int              Data1;
    public int              Data2;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id           = element.GetInt32("Id");
        this.Name         = element.GetString("Name");
        this.ItemType     = (EItemType)element.GetInt32("ItemType");
        this.BagType      = (EBagType)element.GetInt32("BagType");
        this.Quality      = element.GetInt32("Quality");
        this.Icon         = element.GetString("Icon");
        this.SellMoneyId  = element.GetInt32("SellMoneyId");
        this.SellMoneyNum = element.GetInt32("SellMoneyNum");
        this.Desc         = element.GetString("Desc");
        this.Data1        = element.GetInt32("Data1");
        this.Data2        = element.GetInt32("Data2");
        this.Model_R      = element.GetString("Model_R");
        this.Model_L      = element.GetString("Model_L");
        this.Carrer       = element.GetInt32("Carrer");
    }
}


public class ReadCfgItem : DReadBase<int, DItem>
{

}