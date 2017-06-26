using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using Protocol;
using ProtoBuf;

public class RaidCtrl : ICtrl
{
    public void AddListener()
    {
        NetworkManager.AddListener(MessageID.MSG_ACK_PASSCOPY,          OnAck_PassCopy);
        NetworkManager.AddListener(MessageID.MSG_ACK_GET_CHAPTERAWARD,  OnAck_GetChapterAward);
        NetworkManager.AddListener(MessageID.MSG_REQ_BATTLE_CHECK,      OnAck_BattleCheck);
    }

    public void DelListener()
    {
        NetworkManager.DelListener(MessageID.MSG_ACK_PASSCOPY, OnAck_PassCopy);
        NetworkManager.DelListener(MessageID.MSG_ACK_GET_CHAPTERAWARD, OnAck_GetChapterAward);
        NetworkManager.DelListener(MessageID.MSG_REQ_BATTLE_CHECK, OnAck_BattleCheck);
    }

    private void OnAck_BattleCheck(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckBattleCheck ack = Serializer.Deserialize<AckBattleCheck>(ms);

        GTEventCenter.FireEvent(GTEventID.TYPE_BATTLE_CHECK);
        DCopy db = ReadCfgCopy.GetDataById(ack.CopyID);
        GTLauncher.Instance.LoadScene(db.SceneId);
    }

    private void OnAck_GetChapterAward(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckGetChapterAward ack        = Serializer.Deserialize<AckGetChapterAward>(ms);
        int                chapter    = ack.Chapter;
        int                awardIndex = ack.Index;

        DWorld db = ReadCfgWorld.GetDataById(chapter);
        XMainChapter xc;
        if (!DataDBSMainChapter.ContainsKey(chapter))
        {
            xc = new XMainChapter();
            xc.Id = chapter;
        }
        else
        {
            xc = DataDBSMainChapter.GetDataById(chapter);
        }

        switch (awardIndex)
        {
            case 0:
                xc.Award1 = 1;
                break;
            case 1:
                xc.Award2 = 1;
                break;
            case 3:
                xc.Award3 = 1;
                break;
        }

        DataDBSMainChapter.Update(chapter, xc);

        int awardID = db.Awards[awardIndex];
        List<KStruct> list = AwardModule.Instance.GetAwardDataByID(awardID);
        for (int i = 0; i < list.Count; i++)
        {
            DataManager.Instance.AddNewItem(list[i].Id, list[i].Num);
        }

        GTEventCenter.FireEvent(GTEventID.TYPE_GET_CHAPTERAWARD);
    }

    private void OnAck_PassCopy(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckPassCopy   ack = Serializer.Deserialize<AckPassCopy>(ms);
        int chapter  = ack.Chapter;
        int copyID   = ack.CopyID;
        int copyType = ack.CopyType;
        int starNum  = ack.StarNum;

        XCopy copy;
        if (!DataDBSCopy.ContainsKey(copyID))
        {
            copy = new XCopy();
            copy.Id = copyID;
            copy.StarNum = starNum;
        }
        else
        {
            copy = DataDBSCopy.GetDataById(copyID);
            if (copy.StarNum < starNum)
            {
                copy.StarNum = starNum;
            }
        }
        DataDBSCopy.Update(copyID, copy);
        DCopy copyDB = ReadCfgCopy.GetDataById(copyID);
        DataManager.Instance.UseAction(copyDB.CostActionId, copyDB.CostActionNum);
        int key = (int)copyType;
        XRaid raid;
        if (!DataDBSRaid.ContainsKey(key))
        {
            raid = new XRaid();
        }
        else
        {
            raid = DataDBSRaid.GetDataById(key);
        }

        raid.MaxChapter = chapter;
        raid.MaxCopyId = copyID;
        raid.Id = key;
        DataDBSRaid.Update(key, raid);

        if (starNum > 0)
        {
            XCharacter role = RoleModule.Instance.GetCurPlayer();
            RoleService.Instance.TryAddRoleExp(copyDB.GetExpRatio * role.Level);

            DataManager.Instance.AddMoney(copyDB.GetMoneyId, copyDB.GetMoneyRatio * role.Level);
            DataManager.Instance.UseAction(copyDB.CostActionId, copyDB.CostActionNum);
            List<KStruct> list = AwardModule.Instance.GetAwardDataByID(copyDB.AwardId);
            if (list != null)
            {
                DataManager.Instance.AddNewItemList(list, false);
            }
        }

        GTEventCenter.FireEvent(GTEventID.TYPE_PASS_COPY);
    }
}
