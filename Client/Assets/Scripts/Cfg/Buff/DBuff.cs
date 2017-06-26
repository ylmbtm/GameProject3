using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using System.Xml;

public class DBuff : DObj<int>
{
    public int                 Id;
    public string              Name;
    public string              Icon;
    public EBuffType           BuffType;
    public EBuffOverlayType    OverlayType;
    public EBuffDestroyType    DestroyType;

    public float               LifeTime;
    public int                 MaxOverlayNum;
    public int                 EffectID;
    public int                 Sort;

    public EBind               Bind;
    public Int32               ChangeModelID;
    public float               ChangeModelScale;
    public string              Desc;

    public EActType            Result;
    public Int32               ResultAttr;
    public float               ResultInterval;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id               = element.GetInt("Id");
        this.Name             = element.GetString("Name");
        this.Icon             = element.GetString("Icon");
        this.BuffType         = (EBuffType)element.GetInt("BuffType");
        this.OverlayType      = (EBuffOverlayType)element.GetInt("BuffOverType");
        this.DestroyType      = (EBuffDestroyType)element.GetInt("DestroyType");
        this.LifeTime         = element.GetFloat("LifeTime");
        this.MaxOverlayNum    = element.GetInt("MaxOverlayNum");
        this.EffectID         = element.GetInt("EffectID");
        this.Sort             = element.GetInt("Sort");
        this.ChangeModelID    = element.GetInt("ChangeModelID");
        this.ChangeModelScale = element.GetFloat("ChangeModelScale");
        this.Desc             = element.GetString("Desc");
        this.Bind             = (EBind)element.GetInt("EffectBind");
        this.Result           = (EActType)element.GetInt("Result");
        this.ResultAttr       = element.GetInt("ResultAttrID");
        this.ResultInterval   = element.GetFloat("ResultInterval");
    }
}

public class ReadCfgBuff : DReadBase<int, DBuff>
{

}