using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Protocol;

public class EquipService : GTSingleton<EquipService>
{
    public void TryStrengthEquip(XEquip equip, List<XItem> list)
    {
        if (list.Count==0)
        {
            GTItemHelper.ShowTip("请添加装备或者水晶");
            return;
        }
        int addExpNum = EquipModule.Instance.GetStrengthenExpByMatList(list);
        if (!GTItemHelper.CheckItemEnongh(DEquip.EQUIP_STRENGTHEN_MONEY_ID_1, addExpNum))
        {
            return;
        }

        ReqStrengthEquip req = new ReqStrengthEquip();
        req.TarEquip = equip;
        req.UseItems.AddRange(list);
        NetworkManager.Instance.Send<ReqStrengthEquip>(MessageID.MSG_REQ_STRENGTHEN_EQUIP, req, 0, 0);
    }

    public void TryAdvanceEquip(XEquip equip)
    {
        if(EquipModule.Instance.IsFullAdvanceLevel(equip))
        {
            GTItemHelper.ShowTip("进阶等级已满");
            return;
        }
        DEquip cfg       = ReadCfgEquip.GetDataById(equip.Id);
        int      advanceID = cfg.Quality * 1000 + equip.AdvanceLevel + 1;
        DEquipAdvanceCost db = ReadCfgEquipAdvanceCost.GetDataById(advanceID);

        if (!GTItemHelper.CheckItemEnongh(db.CostMoneyId, db.CostMoneyNum))
        {
            return;
        }
        if (!GTItemHelper.CheckItemEnongh(db.CostItemId, db.CostItemNum))
        {
            return;
        }

        List<XItem> list;
        if (db.CostEquipNum > 0)
        {
            list = EquipModule.Instance.GetBagSameEquipList(equip);
            if (list.Count < db.CostEquipNum)
            {
                GTItemHelper.ShowTip("缺少同样的装备");
                return;
            }
            list.RemoveRange(db.CostEquipNum, list.Count - db.CostEquipNum);
        }
        else
        {
            list = new List<XItem>();
        }

        ReqAdvanceEquip req = new ReqAdvanceEquip();
        req.TarEquip = equip;
        req.UseItems.AddRange(list);
        NetworkManager.Instance.Send<ReqAdvanceEquip>(MessageID.MSG_REQ_ADVANCE_EQUIP, req, 0, 0);
    }

    public void TryUpStarEquip(XEquip equip)
    {
        if (EquipModule.Instance.IsFullStarLevel(equip))
        {
            GTItemHelper.ShowTip("装备星级已满");
            return;
        }
        DEquip equipDB = ReadCfgEquip.GetDataById(equip.Id);
        int starID = equipDB.Quality * 1000 + equip.StarLevel + 1;
        DEquipStar db = ReadCfgEquipStar.GetDataById(starID);
        if (!GTItemHelper.CheckItemEnongh(db.CostMoneyId, db.CostMoneyNum))
        {
            return;
        }
        if (!GTItemHelper.CheckItemEnongh(db.CostItemId, db.CostItemNum))
        {
            return;
        }

        ReqUpStarEquip req = new ReqUpStarEquip();
        req.TarEquip = equip;
        NetworkManager.Instance.Send<ReqUpStarEquip>(MessageID.MSG_REQ_UPSTAR_EQUIP, req, 0, 0);
    }
}
