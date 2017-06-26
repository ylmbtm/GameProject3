using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DPartnerAdvance : DObj<int>
{
    public int                        Id;
    public int                        Partner;
    public int                        Advance;
    public int                        CostSoulNum;
    public int                        MainTarget;
    public int                        ViceTarget;
    public Dictionary<EAttr, int>     MainPropertys = new Dictionary<EAttr, int>();
    public Dictionary<EAttr, int>     VicePropertys = new Dictionary<EAttr, int>();
    public string                     Desc1;
    public string                     Desc2;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id          = element.GetInt("Id");
        this.Partner     = element.GetInt("Partner");
        this.Advance     = element.GetInt("Advance");
        this.CostSoulNum = element.GetInt("CostSoulNum");
        this.MainTarget  = element.GetInt("MainTarget");
        this.ViceTarget  = element.GetInt("ViceTarget");
        this.Desc1       = element.GetString("Desc1");
        this.Desc2       = element.GetString("Desc2");

        for (int i = 1; i <= 2; i++)
        {
            int mainID = element.GetInt("MainID" + i);
            int viceID = element.GetInt("ViceID" + i);
            if (mainID > 0)
            {
                EAttr id = (EAttr)mainID;
                int num = element.GetInt("MainNum" + i);
                this.MainPropertys.Add(id, num);

            }
            if (viceID > 0)
            {
                EAttr id = (EAttr)viceID;
                int num = element.GetInt("ViceNum" + i);
                this.VicePropertys.Add(id, num);
            }
        }
    }
}

public class ReadCfgPartnerAdvance : DReadBase<int, DPartnerAdvance>
{

}