using UnityEngine;
using System.Collections;
using System;
using System.Xml;

public class DRole : DObj<int>
{
    public int     ActorID;
    public string  CarrerName = string.Empty;
    public int     Carrer;
    public int     Star1;
    public int     Star2;
    public int     Star3;
    public int     Star4;
    public int     DisplayWeapon;

    public override int GetKey()
    {
        return Carrer;
    }

    public override void Read(XmlElement element)
    {
        this.Carrer        = element.GetInt32("Carrer");
        this.CarrerName    = element.GetString("CarrerName");
        this.ActorID       = element.GetInt32("ActorID");
        this.Star1         = element.GetInt32("Star1");
        this.Star2         = element.GetInt32("Star2");
        this.Star3         = element.GetInt32("Star3");
        this.Star4         = element.GetInt32("Star4");
        this.DisplayWeapon = element.GetInt32("DisplayWeapon");
    }
}

public class ReadCfgRole : DReadBase<int, DRole>
{

}