using UnityEngine;
using System.Collections;
using System;
using Protocol;

public class RaidService : GTSingleton<RaidService>
{
    public bool CheckCopy(ECopyType copyType, int chapter, int copyID)
    {
        int key = (int)copyType;
        DWorld db = ReadCfgWorld.GetDataById(chapter);
        if (DataDBSRaid.ContainsKey(key))
        {
            XRaid raid = DataDBSRaid.GetDataById(key);
            if (chapter > raid.MaxChapter)
            {
                if (chapter != raid.MaxChapter + 1)
                {
                    GTItemHelper.ShowTip(string.Format("章节超出限制:{0}/{1}", chapter, raid.MaxChapter));
                    return false;
                }
                if (copyID != db.Copys[0])
                {
                    GTItemHelper.ShowTip("非法CopyID" + copyID);
                    return false;
                }
            }
            else if (chapter == raid.MaxChapter)
            {
                if (copyID > raid.MaxCopyId + 1)
                {
                    GTItemHelper.ShowTip("非法CopyID" + copyID);
                    return false;
                }
            }
        }
        return true;
    }

    public void TryBattleCheck(ECopyType copyType, int chapter, int copyID)
    {
        if(CheckCopy(copyType,chapter,copyID)==false)
        {
            return;
        }
        DCopy copyDB = ReadCfgCopy.GetDataById(copyID);
        if (GTItemHelper.CheckItemEnongh(copyDB.CostActionId, copyDB.CostActionNum) == false)
        {
            return;
        }


        ReqBattleCheck req = new ReqBattleCheck();
        req.CopyType = (int)copyType;
        req.CopyID = copyID;
        req.Chapter = chapter;
        NetworkManager.Instance.Send<ReqBattleCheck>(MessageID.MSG_REQ_BATTLE_CHECK, req, 0, 0);
    }

    public void TryPassCopy(ECopyType copyType,int chapter,int copyID,int starNum)
    {
        if (starNum < 0 || starNum > 3)
        {
            GTItemHelper.ShowTip("非法StarNum" + starNum);
            return;
        }
        if (CheckCopy(copyType,chapter,copyID)==false)
        {
            return;
        }
        ReqPassCopy req = new ReqPassCopy();
        req.CopyType = (int)copyType;
        req.CopyID = copyID;
        req.Chapter = chapter;
        req.StarNum = starNum;
        NetworkManager.Instance.Send<ReqPassCopy>(MessageID.MSG_REQ_PASSCOPY, req, 0, 0);
    }

    public void TryReceiveReward(ECopyType copyType,int chapter,int awardIndex)
    {
        if(awardIndex<0 || awardIndex>2)
        {
            GTItemHelper.ShowTip("非法AwardIndex" + awardIndex);
            return;
        }
        int rewardState = RaidModule.Instance.GetChapterRewardStateByAwardIndex(chapter, awardIndex);     
        switch(rewardState)
        {
            case ERewardState.NOT_RECEIVE:
                GTItemHelper.ShowTip("未达成条件");
                return;
            case ERewardState.HAS_RECEIVE:
                GTItemHelper.ShowTip("奖励已领取");
                return;
        }
        DWorld chapterDB = ReadCfgWorld.GetDataById(chapter);
        DAward awardDB = ReadCfgAward.GetDataById(chapterDB.Awards[awardIndex]);
        if(GTItemHelper.CheckBagFull(awardDB.MaxDropNum))
        {
            return;
        }

        ReqGetChapterAward req = new ReqGetChapterAward();
        req.CopyType = (int)copyType;
        req.Chapter = chapter;
        req.Index = awardIndex;
        NetworkManager.Instance.Send<ReqGetChapterAward>(MessageID.MSG_REQ_GET_CHAPTERAWARD, req, 0, 0);
    }
}
