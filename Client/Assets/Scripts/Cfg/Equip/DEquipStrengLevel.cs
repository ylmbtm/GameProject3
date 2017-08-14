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
        this.Id         = element.GetInt32("Id");
        this.Quality    = element.GetInt32("Quality");
        this.Level      = element.GetInt32("Level");
        this.RequireExp = element.GetInt32("RequireExp");
    }
}

public class ReadCfgEquipStrengLevel : DReadBase<int,DEquipStrengLevel>
{

}