using UnityEngine;
using System.Collections;
using System;
using Protocol;
using ProtoBuf;
using System.IO;

public class RoleCtrl : ICtrl
{
    public void AddListener()
    {
        NetworkManager.AddListener(MessageID.MSG_ACK_ADDHERO_EXP,         OnAck_AddHeroExp);
    }

    public void DelListener()
    {
        NetworkManager.DelListener(MessageID.MSG_ACK_ADDHERO_EXP,         OnAck_AddHeroExp);
    }

    private void OnAck_AddHeroExp(MessageRecv obj )
    {
        System.IO.MemoryStream ms = new System.IO.MemoryStream(obj.Data);
        ms.Seek(28, SeekOrigin.Begin);
        AckAddPlayerExp ack = Serializer.Deserialize<AckAddPlayerExp>(ms);

        XCharacter player = RoleModule.Instance.GetCurPlayer();
        int oldLevel = player.Level;

        int maxLevel = ReadCfgRoleLevel.Count;
        if (player.Level >= maxLevel)
        {
            return;
        }
        player.CurExp += ack.Exp;
        DRoleLevel levelDB = ReadCfgRoleLevel.GetDataById(player.Level);
        while (player.CurExp >= levelDB.RequireExp)
        {
            player.CurExp -= levelDB.RequireExp;
            player.Level++;
            if (player.Level >= maxLevel)
            {
                player.CurExp = 0;
                break;
            }
            levelDB = ReadCfgRoleLevel.GetDataById(player.Level);
        }
        DataDBSRole.Update(player.Id, player);
        GTItemHelper.ShowExpTip(ack.Exp);


        int newLevel = player.Level;
        if (newLevel > oldLevel)
        {
            CharacterManager.Instance.SyncMainPlayerData(ESyncDataType.TYPE_LEVEL);
            GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_HEROLEVEL);
            GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_FIGHTVALUE);
        }
        GTEventCenter.FireEvent(GTEventID.TYPE_CHANGE_HEROEXP);
    }
}

