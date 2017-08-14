using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

public class DQuality : DObj<int>
{
    public int    Quality;
    public string Name;
    public string Icon;
    public string Encode;
    public string Desc;

    public override int GetKey()
    {
        return Quality;
    }

    public override void Read(XmlElement element)
    {
        this.Quality = element.GetInt32("Quality");
        this.Name    = element.GetString("Name");
        this.Icon    = element.GetString("Icon");
        this.Encode  = element.GetString("Encode");
        this.Desc    = element.GetString("Desc");
    }
}

public class ReadCfgQuality : DReadBase<int, DQuality>
{

}