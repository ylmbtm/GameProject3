using UnityEngine;
using System.Collections;
using Protocol;

public class MLRaid : GTSingleton<MLRaid>
{
    public ERewardState GetChapterRewardStateByAwardIndex(int chapter, int index)
    {
        if (!DataDBSMainChapter.ContainsKey(chapter))
        {
            return ERewardState.NOT_RECEIVE;
        }
        XMainChapter vo = DataDBSMainChapter.GetDataById(chapter);
        DCopyMainChapter db = ReadCfgCopyMainChapter.GetDataById(chapter);
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

    public int GetMaxChapterByDifficulty(int difficulty)
    {
        XRaid raid = DataDBSRaid.GetDataById(difficulty);
        return raid == null ? DCopyMainChapter.MAINCOPY_ST_CHAPTER_ID : raid.MaxChapter;
    }

    public int GetMaxCopyIDByDifficulty(int difficulty)
    {
        XRaid raid = DataDBSRaid.GetDataById(difficulty);
        return raid == null ? 0 : raid.MaxCopyId;
    }

    public int GetMaxCanEnterCopyIndex(int difficulty, int chapter)
    {
        int maxCopyID = GetMaxCopyIDByDifficulty(difficulty);
        int maxChapter = GetMaxChapterByDifficulty(difficulty);
        DCopyMainChapter db = ReadCfgCopyMainChapter.GetDataById(chapter);

        if (maxChapter == 0 && chapter == DCopyMainChapter.MAINCOPY_ST_CHAPTER_ID)
        {
            return 1;
        }
        if (maxChapter > chapter)
        {
            return db.Copys.Length;
        }
        if (maxChapter == chapter)
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

    public int GetMaxCanEnterChapter(int difficulty)
    {
        int chapter = 0;
        if (DataDBSRaid.ContainsKey(difficulty))
        {
            XRaid raid = DataDBSRaid.GetDataById(difficulty);
            DCopyMainChapter chapterDB = ReadCfgCopyMainChapter.GetDataById(raid.MaxChapter);
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
            chapter = DCopyMainChapter.MAINCOPY_ST_CHAPTER_ID;
        }
        return chapter;
    }

    public int GetNextChapter(int chapter)
    {
        if (ReadCfgCopyMainChapter.ContainsKey(chapter + 1))
        {
            return chapter + 1;
        }
        return chapter;
    }

    public int GetPrevChapter(int chapter)
    {
        if (ReadCfgCopyMainChapter.ContainsKey(chapter - 1))
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
            DCopyMainChapter db = ReadCfgCopyMainChapter.GetDataById(chapter);
            for (int i = 0; i < db.Copys.Length; i++)
            {
                starNum += GetCopyStarNumById(db.Copys[i]);
            }
        }
        return starNum;
    }
}
