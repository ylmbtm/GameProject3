using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using Protocol;
using ProtoBuf;

public class GemCtrl : ICtrl
{
    public void AddListener()
    {
        NetworkManager.AddListener(MessageID.MSG_ACK_STRENGTHEN_GEM, OnAck_StrengthGem);
    }

    public void DelListener()
    {
        NetworkManager.DelListener(MessageID.MSG_ACK_STRENGTHEN_GEM, OnAck_StrengthGem);
    }

    private void OnAck_StrengthGem(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        AckStrengthGem ack  = Serializer.Deserialize<AckStrengthGem>(ms);
        XGem           gem  = ack.TarGem;
        List<XItem>    list = ack.UseItems;

        DGem gemDB        = ReadCfgGem.GetDataById(gem.Id);
        int addExpNum       = GemModule.Instance.GetExpByStrengthMatList(list);
        int strengthenLevel = gem.StrengthenLevel;

        int levelID = gemDB.Quality * 1000 + strengthenLevel + 1;
        DGemLevel db = ReadCfgGemLevel.GetDataById(levelID);
        int expNum = db.RequireExp - gem.StrengthenExp;
        while (addExpNum >= expNum)
        {
            strengthenLevel++;
            levelID = gemDB.Quality * 1000 + strengthenLevel + 1;
            db = ReadCfgGemLevel.GetDataById(levelID);
            expNum += db.RequireExp;
        }
        gem.StrengthenLevel = strengthenLevel;
        gem.StrengthenExp = addExpNum + db.RequireExp - expNum;

        DataDBSGem.Update(gem.Instance, gem);
        DataManager.Instance.UseMoney(db.CostMoneyId, expNum);
        for (int i = 0; i < list.Count; i++)
        {
            XItem item = list[i];
            EPosType posType = (EPosType)item.PosType;
            switch (posType)
            {
                case EPosType.BagItem:
                    DataManager.Instance.UseItemById(item.Id, item.Num);
                    break;
                case EPosType.BagGem:
                    DataManager.Instance.DelBagGem(item.Pos);
                    break;
            }
        }

        CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_BASEATTR);
        GTEventCenter.FireEvent(GTEventID.TYPE_STRENGTHEN_GEM);
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
    }
}