using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DWorld : DObj<int>
{
    public int        Id;
    public string     Name;
    public string     Icon;
    public string     Map;
    public ECopyType  CopyType;
    public int[]      Copys;
    public int[]      Stars;
    public int[]      Awards;
    public Vector2[]  CopyPosArray;
    public string     Desc;

    public override int GetKey()
    {
        return Id;
    }

    public override void Read(XmlElement element)
    {
        this.Id           = element.GetInt("Id");
        this.Name         = element.GetString("Name");
        this.Icon         = element.GetString("Icon");
        this.Map          = element.GetString("Map");
        this.CopyType     = (ECopyType)element.GetInt("Type");
        this.Desc         = element.GetString("Desc");

        int stCopyId    = element.GetInt("StCopyId");
        int edCopyId    = element.GetInt("EdCopyId");
        int copyNum     = edCopyId - stCopyId + 1;
        this.Copys        = new int[copyNum];
        this.CopyPosArray = new Vector2[copyNum];
        this.Stars = new int[3];
        this.Awards = new int[3];

        for (int i = stCopyId; i <= edCopyId; i++)
        {
            int index = i - stCopyId;
            this.Copys[index] = i;
            string field = element.GetString("Pos" + (index + 1));
            this.CopyPosArray[index] = field.ToVector2();
        }
        for (int i = 0; i < 3; i++)
        {
            string s1    = "Star" + (i + 1);
            string s2    = "AwardId" + (i + 1);
            this.Stars[i]  = element.GetInt(s1);
            this.Awards[i] = element.GetInt(s2);
        }
    }

    public const int WORLD_ST_CHAPTER_ID = 1;
    public const int WORLD_ED_CHAPTER_ID = 13;
}

public class ReadCfgWorld : DReadBase<int, DWorld>
{

}