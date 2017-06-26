using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DEquipStrengLevel : DObj<int>
{
    public int Id;
    public int Quality;
    public int Level;
    public int RequireExp;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id         = element.GetInt("Id");
        this.Quality    = element.GetInt("Quality");
        this.Level      = element.GetInt("Level");
        this.RequireExp = element.GetInt("RequireExp");
    }
}

public class ReadCfgEquipStrengLevel : DReadBase<int,DEquipStrengLevel>
{

}