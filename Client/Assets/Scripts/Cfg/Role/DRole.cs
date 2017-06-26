using UnityEngine;
using System.Collections;
using System;
using System.Xml;

public class DRole : DObj<int>
{
    public int    Id;
    public string CarrerName = string.Empty;
    public int    Star1;
    public int    Star2;
    public int    Star3;
    public int    Star4;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id         = element.GetInt("Id");
        this.CarrerName = element.GetString("CarrerName ");
        this.Star1      = element.GetInt("Star1");
        this.Star2      = element.GetInt("Star2");
        this.Star3      = element.GetInt("Star3");
        this.Star4      = element.GetInt("Star4");
    }
}

public class ReadCfgRole : DReadBase<int, DRole>
{

}