using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

public class DRoleLevel : DObj<int>
{
    public int                        Level;
    public int                        RequireExp;
    public int                        NextLevel;
    public Dictionary<EAttr, int>     Propertys = new Dictionary<EAttr, int>();

    public override int GetKey()
    {
        return Level;
    }

    public override void Read(XmlElement element)
    {
        this.Level      = element.GetInt("Level");
        this.RequireExp = element.GetInt("RequireExp");
        this.NextLevel  = element.GetInt("NextLevel");
        for (int i = 1; i <= 10; i++)
        {
            int value = element.GetInt("P" + i);
            this.Propertys.Add((EAttr)i, value);
        }
    }
}

public class ReadCfgRoleLevel : DReadBase<int, DRoleLevel>
{

}
