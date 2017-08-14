using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Text.RegularExpressions;
using System.Xml;

public class DRune : DObj<int>
{
    public int                          Id;
    public EAttr                        PropertyId;
    public int                          PropertyNum;
    public ERuneType                    RuneType;
    public int                          ComposeCostMoneyId;
    public int                          ComposeCostMoneyNum;
    public int                          UpCostMoneyId;
    public List<KeyValuePair<int, int>> Composes     = new List<KeyValuePair<int, int>>();
    public int[]                        UpValues     = new int[5] { 0, 0, 0, 0, 0 };
    public int[]                        UpCostMoneys = new int[5] { 0, 0, 0, 0, 0 };

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id                  = element.GetInt32("Id");
        this.PropertyId          = (EAttr)element.GetInt32("PropertyId");
        this.PropertyNum         = element.GetInt32("PropertyNum");
        this.RuneType            = (ERuneType)element.GetInt32("RuneType");
        this.ComposeCostMoneyId  = element.GetInt32("ComposeCostMoneyId");
        this.ComposeCostMoneyNum = element.GetInt32("ComposeCostMoneyNum");
        this.UpCostMoneyId       = element.GetInt32("UpCostMoneyId");
        for (int i = 1; i <= 5; i++)
        {
            this.UpCostMoneys[i - 1] = element.GetInt32("UpCostMoney" + i);
        }
        for (int i = 1; i <= 5; i++)
        {
            this.UpValues[i - 1] = element.GetInt32("PropertyLevel" + i);
        }
        string compose           = element.GetString("Compose");
        if (compose.Contains("|"))
        {
            Regex reg = new Regex(@"(?is)(?<=\()[^\)]+(?=\))");
            MatchCollection mc = reg.Matches(compose);
            for (int i = 0; i < mc.Count; i++)
            {
                string s = mc[i].Value;
                string[] array = s.Split('|');
                if (array.Length != 2)
                {
                    continue;
                }
                KeyValuePair<int, int> kv = new KeyValuePair<int, int>(array[1].ToInt32(), array[2].ToInt32());
                this.Composes.Add(kv);
            }
        }
    }
}

public class ReadCfgRune : DReadBase<int, DRune>
{

}
