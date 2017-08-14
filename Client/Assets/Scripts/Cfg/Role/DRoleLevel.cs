using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

public class DRoleLevel : DObj<int>
{
    public int                        Level;
    public int                        RequireExp;
    public int                        NextLevel;
    public Dictionary<EAttr, int>     Attrs = new Dictionary<EAttr, int>();

    public override int GetKey()
    {
        return Level;
    }

    public override void Read(XmlElement element)
    {
        this.Level      = element.GetInt32("Level");
        this.RequireExp = element.GetInt32("RequireExp");
        this.NextLevel  = element.GetInt32("NextLevel");
        for (int i = 1; i <= 10; i++)
        {
            int value = element.GetInt32("P" + i);
            this.Attrs.Add((EAttr)i, value);
        }
    }
}

public class ReadCfgRoleLevel : DReadBase<int, DRoleLevel>
{

}
