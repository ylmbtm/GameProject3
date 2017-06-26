using UnityEngine;
using System.Collections;
using Protocol;

public class RaidModule : GTSingleton<RaidModule>
{
    public int GetChapterRewardStateByAwardIndex(int chapter, int index)
    {
        if (!DataDBSMainChapter.ContainsKey(chapter))
        {
            return ERewardState.NOT_RECEIVE;
        }
        XMainChapter vo = DataDBSMainChapter.GetDataById(chapter);
        DWorld db = ReadCfgWorld.GetDataById(chapter);
        int starNum = GetChapterStarNumByChapter(chapter);
        if (starNum < db.Stars[index])
        {
            return ERewardState.NOT_RECEIVE;
        }
        int s = 0;
        switch (index)
        {
            case 0:
                s = vo.Award1;
                break;
            case 1:
                s = vo.Award2;
                break;
            case 2:
                s = vo.Award3;
                break;
        }
        return s == 1 ? ERewardState.HAS_RECEIVE : ERewardState.CAN_RECEIVE;
    }

    public int GetMaxChapterByCopyType(ECopyType copyType)
    {
        int key = (int)copyType;
        XRaid raid = DataDBSRaid.GetDataById(key);
        return raid == null ? DWorld.WORLD_ST_CHAPTER_ID : raid.MaxChapter;
    }

    public int GetMaxCopyIdByCopyType(ECopyType copyType)
    {
        int key = (int)copyType;
        XRaid raid = DataDBSRaid.GetDataById(key);
        return raid == null ? 0 : raid.MaxCopyId;
    }

    public int GetMaxCanEnterCopyIndex(ECopyType copyType, int chapterID)
    {
        int maxCopyID = GetMaxCopyIdByCopyType(copyType);
        int maxChapter = GetMaxChapterByCopyType(copyType);
        DWorld db = ReadCfgWorld.GetDataById(chapterID);

        if (maxChapter == 0 && chapterID == DWorld.WORLD_ST_CHAPTER_ID)
        {
            return 1;
        }
        if (maxChapter > chapterID)
        {
            return db.Copys.Length;
        }
        if (maxChapter == chapterID)
        {
            if (maxCopyID >= 0)
            {
                int index = 1;
                for (int i = 0; i < db.Copys.Length; i++)
                {
                    if (db.Copys[i] <= maxCopyID)
                    {
                        index++;
                    }
                    else
                    {
                        break;
                    }
                }
                index = index > db.Copys.Length ? db.Copys.Length : index;
                return index;
            }
        }
        return 0;
    }

    public int GetMaxCanEnterChapter(ECopyType copyType)
    {
        int key = (int)copyType;
        int chapter = 0;
        if (DataDBSRaid.ContainsKey(key))
        {
            XRaid raid = DataDBSRaid.GetDataById(key);
            DWorld chapterDB = ReadCfgWorld.GetDataById(raid.MaxChapter);
            if (chapterDB.Copys[chapterDB.Copys.Length - 1] == raid.MaxCopyId)
            {
                chapter = GetNextChapter(raid.MaxChapter);
            }
            else
            {
                chapter = raid.MaxChapter;
            }
        }
        else
        {
            chapter = DWorld.WORLD_ST_CHAPTER_ID;
        }
        return chapter;
    }

    public int GetNextChapter(int chapter)
    {
        if (ReadCfgWorld.ContainsKey(chapter + 1))
        {
            return chapter + 1;
        }
        return chapter;
    }

    public int GetPrevChapter(int chapter)
    {
        if (ReadCfgWorld.ContainsKey(chapter - 1))
        {
            return chapter - 1;
        }
        return chapter;
    }

    public int GetCopyStarNumById(int id)
    {
        if (DataDBSCopy.ContainsKey(id))
        {
            return DataDBSCopy.GetDataById(id).StarNum;
        }
        else
        {
            return 0;
        }
    }

    public int GetChapterStarNumByChapter(int chapter)
    {
        int starNum = 0;
        if (DataDBSMainChapter.ContainsKey(chapter))
        {
            DWorld db = ReadCfgWorld.GetDataById(chapter);
            for (int i = 0; i < db.Copys.Length; i++)
            {
                starNum += GetCopyStarNumById(db.Copys[i]);
            }
        }
        return starNum;
    }
}
