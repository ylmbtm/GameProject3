using UnityEngine;
using System.Collections;
using System;
using System.Xml;

public class DSearch : DObj<int>
{
    public int   Id;
    public int   MapID;
    public int   Type;
    public int   TypeId;
    public float PosX;
    public float PosY;
    public float PosZ;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id      = element.GetInt32("Id");
        this.MapID   = element.GetInt32("MapID");
        this.Type    = element.GetInt32("Type");
        this.TypeId  = element.GetInt32("TypeId");
        this.PosX    = element.GetFloat("PosX");
        this.PosY    = element.GetFloat("PosY");
        this.PosZ    = element.GetFloat("PosZ");
    }
}

public class ReadCfgSearch : DReadBase<int, DSearch>
{

}