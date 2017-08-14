using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

public class DProperty : DObj<EAttr>
{
    public EAttr       Id;
    public string      Name;
    public int         Factor;
    public bool        IsPercent;
    public string      Desc;

    public override EAttr GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id        = (EAttr)element.GetInt32("Id");
        this.Name      = element.GetString("Name");
        this.Factor    = element.GetInt32("Factor");
        this.Desc      = element.GetString("Desc");
        this.IsPercent = element.GetBool("IsPercent");
    }
}

public class ReadCfgProperty : DReadBase<EAttr, DProperty>
{

}