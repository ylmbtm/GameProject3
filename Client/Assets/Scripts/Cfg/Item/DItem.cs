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
    public ECarrer          Carrer;
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
        this.Id           = element.GetInt("Id");
        this.Name         = element.GetString("Name");
        this.ItemType     = (EItemType)element.GetInt("ItemType");
        this.BagType      = (EBagType)element.GetInt("BagType");
        this.Quality      = element.GetInt("Quality");
        this.Icon         = element.GetString("Icon");
        this.SellMoneyId  = element.GetInt("SellMoneyId");
        this.SellMoneyNum = element.GetInt("SellMoneyNum");
        this.Desc         = element.GetString("Desc");
        this.Data1        = element.GetInt("Data1");
        this.Data2        = element.GetInt("Data2");
        this.Model_R      = element.GetString("Model_R");
        this.Model_L      = element.GetString("Model_L");
        this.Carrer       = (ECarrer)element.GetInt("Carrer");
    }
}


public class ReadCfgItem : DReadBase<int, DItem>
{

}