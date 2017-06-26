using UnityEngine;
using System.Collections;
using Protocol;

public class RelicsService : GTSingleton<RelicsService>
{
    public void TryChargeRelics(int relicsID, int index)
    {
        DRelics db = ReadCfgRelics.GetDataById(relicsID);
        if (db == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        if (index > 3 || index < 1)
        {
            GTItemHelper.ShowTip("非法索引" + index.ToString());
            return;
        }
        XRelics relics = DataDBSRelics.GetDataById(relicsID);
        if (relics != null)
        {
            if (RelicsModule.Instance.GetExp(relics, index) >= db.LevelRequireExp[relics.Level])
            {
                GTItemHelper.ShowTip("你不能对此充能了");
                return;
            }
        }
        if (GTItemHelper.CheckItemEnongh(db.ArtificeCostID[index - 1], 1) == false)
        {
            return;
        }

        ReqChargeRelics req = new ReqChargeRelics();
        req.RelicsID = relicsID;
        req.Index = index;
        NetworkManager.Instance.Send(MessageID.MSG_REQ_CHARGE_RELICS, req, 0, 0);
    }

    public void TryUpgradeRelics(int relicsID)
    {
        DRelics db = ReadCfgRelics.GetDataById(relicsID);
        if (db == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        XRelics relics = DataDBSRelics.GetDataById(relicsID);
        if (relics == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        if (relics.Level == db.LevelRequireExp.Length)
        {
            GTItemHelper.ShowTip("已升到最大等级，无法继续升级");
            return;
        }
        for (int i = 0; i < 3; i++)
        {
            if (RelicsModule.Instance.GetExp(relics, i + 1) < db.LevelRequireExp[relics.Level])
            {
                GTItemHelper.ShowTip("神器未获得足够的充能");
                return;
            }
        }
        ReqUpgradeRelics req = new ReqUpgradeRelics();
        req.RelicsID = relicsID;
        NetworkManager.Instance.Send(MessageID.MSG_REQ_UPGRADE_RELICS, req, 0, 0);
    }

    public void TryBattleRelics(int relicsID)
    {
        DRelics db = ReadCfgRelics.GetDataById(relicsID);
        if (db == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        XCharacter role = RoleModule.Instance.GetCurPlayer();
        if (role.Relic == relicsID)
        {
            GTItemHelper.ShowTip("该神器已经上阵");
            return;
        }

        ReqBattleRelics req = new ReqBattleRelics();
        req.RelicsID = relicsID;
        NetworkManager.Instance.Send(MessageID.MSG_REQ_BATTLE_RELICS, req, 0, 0);
    }

    public void TryUnloadRelics(int relicsID)
    {
        DRelics db = ReadCfgRelics.GetDataById(relicsID);
        if (db == null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        XCharacter role = RoleModule.Instance.GetCurPlayer();
        if (role.Relic != relicsID)
        {
            GTItemHelper.ShowTip("该神器没有上阵，无法卸下");
            return;
        }
        ReqUnloadRelics req = new ReqUnloadRelics();
        req.RelicsID = relicsID;
        NetworkManager.Instance.Send(MessageID.MSG_REQ_UNLOAD_RELICS, req, 0, 0);
    }
}
