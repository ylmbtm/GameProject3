using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;


public class DCopy : DObj<int>
{
    public int                 Id;
    public ECopyType           CopyType;
    public int                 CopyDiffculty;
    public string              Name;
    public int                 CostActionId;
    public int                 CostActionNum;
    public int                 AwardId;
    public int                 FirstAwardId;
    public int                 GetMoneyId;
    public int                 GetMoneyRatio;
    public int                 GetExpRatio;
    public int                 BattleTimes;
    public int                 UnlockLevel;
    public string              FightValue;
    public string              Desc;
    public string              Texture;
    public string              Icon;
    public int                 DropBoxAwardId;
    public string              SceneName;
    public string              SceneMusic;
    public string              SceneConfig;
    public string              SceneNavMesh;


    public EStarCondition[]    StarConditions = new EStarCondition[3];
    public int[]               StarValues     = new int[3];

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id                 = element.GetInt32("Id");
        this.CopyType           = (ECopyType)element.GetInt32("CopyType");
        this.CopyDiffculty      = element.GetInt32("CopyDiffculty");
        this.Name               = element.GetString("Name");
        this.CostActionId       = element.GetInt32("CostActionId");
        this.CostActionNum      = element.GetInt32("CostActionNum");
        this.AwardId            = element.GetInt32("AwardId");
        this.FirstAwardId       = element.GetInt32("FirstAwardId");
        this.DropBoxAwardId     = element.GetInt32("DropBoxAwardId");
        this.GetMoneyId         = element.GetInt32("GetMoneyId");
        this.GetMoneyRatio      = element.GetInt32("GetMoneyRatio");
        this.GetExpRatio        = element.GetInt32("GetExpRatio");
        this.BattleTimes        = element.GetInt32("BattleTimes");
        this.UnlockLevel        = element.GetInt32("UnlockLevel");
        this.FightValue         = element.GetString("FightValue");
        this.Desc               = element.GetString("Desc");
        this.Texture            = element.GetString("Texture");
        this.Icon               = element.GetString("Icon");
        this.SceneName          = element.GetString("SceneName");
        this.SceneConfig        = element.GetString("SceneConfig");
        this.SceneMusic         = element.GetString("SceneMusic");
        this.SceneNavMesh       = element.GetString("SceneNavMesh");

        for (int i = 1; i <= 3; i++)
        {
            this.StarValues[i - 1] = element.GetInt32("StarValue" + i);
            this.StarConditions[i - 1] = (EStarCondition)element.GetInt32("StarCondition" + i);
        }
    }
}

public class ReadCfgCopy : DReadBase<int, DCopy>
{
   
}
