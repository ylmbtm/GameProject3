using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DPartnerWash : DObj<int>
{
    public int   Level;
    public int[] PropertyMins = new int[3];
    public int[] PropertyMaxs = new int[3];

    public override int GetKey()
    {
        return Level;
    }

    public override void Read(XmlElement element)
    {
        this.Level = element.GetInt32("Level");
        for (int i = 1; i <= 3; i++)
        {
            this.PropertyMins[i - 1] = element.GetInt32("PropertyMin" + i);
            this.PropertyMaxs[i - 1] = element.GetInt32("PropertyMax" + i);
        }
    }
}

public class ReadCfgPartnerWash : DReadBase<int, DPartnerWash>
{

}