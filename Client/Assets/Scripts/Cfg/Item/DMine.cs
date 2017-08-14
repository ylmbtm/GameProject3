using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DMine : DObj<int>
{
    public int       Id;
    public string    Name = string.Empty;
    public string    Model = string.Empty;
    public int       CostItemID;
    public float     CostTime;
    public float     Radius;
    public int       DropItemID;
    public bool      IsShowMineBar;
    public bool      IsShowName;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id            = element.GetInt32("Id");
        this.Name          = element.GetString("Name");
        this.Model         = element.GetString("Model");
        this.CostItemID    = element.GetInt32("CostItemID");
        this.CostTime      = element.GetFloat("CostTime");
        this.Radius        = element.GetFloat("Radius");
        this.DropItemID    = element.GetInt32("DropItemID");
        this.IsShowMineBar = element.GetBool("IsShowMineBar");
        this.IsShowName    = element.GetBool("IsShowName");
    }
}

public class ReadCfgMine : DReadBase<int, DMine>
{

}
