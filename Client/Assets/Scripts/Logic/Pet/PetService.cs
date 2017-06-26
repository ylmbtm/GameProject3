using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Protocol;

public class PetService : GTSingleton<PetService>
{
    public void TryUpgradePet(int id, List<XItem> items)
    {
        if (items == null || items == null)
        {
            GTItemHelper.ShowTip("缺少食物");
            return;
        }
        for (int i = 0; i < items.Count; i++)
        {
            if (PetModule.Instance.IsFood(items[i].Id) == false)
            {
                GTItemHelper.ShowTip("加入的食品列表有误");
                return;
            }
        }
        DActor db = ReadCfgActor.GetDataById(id);
        XPet data = DataDBSPet.GetDataById(id);
        if (data != null)
        {
            int count = 0;
            foreach (var current in ReadCfgPetLevel.Dict)
            {
                if (current.Value.Quality == db.Quality)
                {
                    count++;
                }
            }

            if (data.Level >= count)
            {
                GTItemHelper.ShowTip("宠物等级已满");
                return;
            }
        }

        ReqUpgradePet req = new ReqUpgradePet();
        req.ID = id;
        req.UseItems.AddRange(items);
        NetworkManager.Instance.Send<ReqUpgradePet>(MessageID.MSG_REQ_UPGRADE_PET, req, 0, 0);
    }

    public void TryBattlePet(int id)
    {
        XCharacter role = RoleModule.Instance.GetCurPlayer();
        if (role.PetID == id)
        {
            GTItemHelper.ShowTip("你的伙伴已上阵");
            return;
        }

        ReqBattlePet req = new ReqBattlePet();
        req.ID = id;
        NetworkManager.Instance.Send<ReqBattlePet>(MessageID.MSG_REQ_BATTLE_PET, req, 0, 0);
    }

    public void TryUnloadPet(int id)
    {
        XCharacter role = RoleModule.Instance.GetCurPlayer();
        if (role.PetID !=id)
        {
            GTItemHelper.ShowTip("你没有上阵该伙伴");
            return;
        }

        ReqUnloadPet req = new ReqUnloadPet();
        req.ID = id;
        NetworkManager.Instance.Send<ReqUnloadPet>(MessageID.MSG_REQ_UNLOAD_PET, req, 0, 0);
    }
}
