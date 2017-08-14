using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using System.Xml;

public class DCopyMainChapter : DObj<int>
{
    public int        Id;
    public string     Name;
    public string     Icon;
    public string     Map;
    public int        CopyDiffculty;
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
        this.Id            = element.GetInt32("Id");
        this.Name          = element.GetString("Name");
        this.Icon          = element.GetString("Icon");
        this.Map           = element.GetString("Map");
        this.CopyDiffculty = element.GetInt32("Type");
        this.Desc          = element.GetString("Desc");

        int stCopyId       = element.GetInt32("StCopyId");
        int edCopyId       = element.GetInt32("EdCopyId");
        int copyNum        = edCopyId - stCopyId + 1;
        this.Copys         = new int[copyNum];
        this.CopyPosArray  = new Vector2[copyNum];
        this.Stars         = new int[3];
        this.Awards        = new int[3];

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
            this.Stars[i]  = element.GetInt32(s1);
            this.Awards[i] = element.GetInt32(s2);
        }
    }

    public const int MAINCOPY_ST_CHAPTER_ID = 1;
    public const int MAINCOPY_ED_CHAPTER_ID = 13;
}

public class ReadCfgCopyMainChapter : DReadBase<int, DCopyMainChapter>
{

}