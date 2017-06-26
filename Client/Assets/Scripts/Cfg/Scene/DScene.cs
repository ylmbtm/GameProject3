using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DScene : DObj<int>
{
    public int         Id;
    public string      SceneName;
    public ESceneType  SceneType;
    public string      SceneMusic;
    public float       DelayTime;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id         = element.GetInt("Id");
        this.SceneName  = element.GetString("SceneName");
        this.SceneType  = (ESceneType)element.GetInt("SceneType");
        this.SceneMusic = element.GetString("SceneMusic");
    }
}

public class ReadCfgScene : DReadBase<int, DScene>
{

}