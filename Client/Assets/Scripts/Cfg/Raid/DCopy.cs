using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public enum ECopyType
{
    TYPE_WORLD = 0,
    TYPE_EASY = 1,
    TYPE_ELITE,
    TYPE_EPIC,
    TYPE_DAILY,
    TYPE_ARENA
}


public enum EStarCondition
{
    TYPE_PASSCOPY,
    TYPE_MAIN_HEALTH,
    TYPE_TIME_LIMIT,
}

public class DCopy : DObj<int>
{
    public int                 Id;
    public ECopyType           CopyType;
    public string              Name;
    public int                 CostActionId;
    public int                 CostActionNum;
    public int                 AwardId;
    public int                 FirstAwardId;
    public int                 SceneId;
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

    public EStarCondition[]    StarConditions = new EStarCondition[3];
    public int[]               StarValues     = new int[3];

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id                 = element.GetInt("Id");
        this.CopyType           = (ECopyType)element.GetInt("CopyType");
        this.Name               = element.GetString("Name");
        this.CostActionId       = element.GetInt("CostActionId");
        this.CostActionNum      = element.GetInt("CostActionNum");
        this.AwardId            = element.GetInt("AwardId");
        this.FirstAwardId       = element.GetInt("FirstAwardId");
        this.DropBoxAwardId     = element.GetInt("DropBoxAwardId");
        this.SceneId            = element.GetInt("SceneId");
        this.GetMoneyId         = element.GetInt("GetMoneyId");
        this.GetMoneyRatio      = element.GetInt("GetMoneyRatio");
        this.GetExpRatio        = element.GetInt("GetExpRatio");
        this.BattleTimes        = element.GetInt("BattleTimes");
        this.UnlockLevel        = element.GetInt("UnlockLevel");
        this.FightValue         = element.GetString("FightValue");
        this.Desc               = element.GetString("Desc");
        this.Texture            = element.GetString("Texture");
        this.Icon               = element.GetString("Icon");
        for (int i = 1; i <= 3; i++)
        {
            this.StarValues[i - 1] = element.GetInt("StarValue" + i);
            this.StarConditions[i - 1] = (EStarCondition)element.GetInt("StarCondition" + i);
        }
    }
}

public class ReadCfgCopy : DReadBase<int, DCopy>
{
   
}
