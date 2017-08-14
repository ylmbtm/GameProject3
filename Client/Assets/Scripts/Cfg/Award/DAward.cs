using System;
using System.Collections.Generic;
using System.Xml;

public class DAward : DObj<int>
{
    public int        Id;
    public string     Name;
    public EDropType  DropType;
    public ERecvType  RecvType;
    public string     DropItems;
    public int        MaxDropNum;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id         = element.GetInt32("Id");
        this.Name       = element.GetString("Name");
        this.DropType   = (EDropType)element.GetInt32("DropType");
        this.RecvType   = (ERecvType)element.GetInt32("RecvType");
        this.DropItems  = element.GetString("DropItems");
        this.MaxDropNum = element.GetInt32("MaxDropNum");
    }
}

public class ReadCfgAward : DReadBase<int, DAward>
{

}
