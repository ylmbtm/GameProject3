using UnityEngine;
using System.Collections;
using System;
using System.Xml;

public class DStage : DObj<int>
{
    public int   Id;
    public float X;
    public float Y;
    public float Z;
    public float Scale;

    public override int  GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id         = element.GetInt32("Id");
        this.X          = element.GetFloat("X");
        this.Y          = element.GetFloat("Y");
        this.Z          = element.GetFloat("Z");
        this.Scale      = element.GetFloat("Scale");
    }
}