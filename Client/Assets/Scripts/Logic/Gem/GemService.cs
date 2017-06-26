using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Protocol;

public class GemService : GTSingleton<GemService>
{
    public void TryStrengthGem(XGem gem, List<XItem> list)
    {
        if (GemModule.Instance.IsFullStrengthenLevel(gem))
        {
            GTItemHelper.ShowTip("宝石强化等级已满");
            return;
        }
        if (list.Count == 0)
        {
            GTItemHelper.ShowTip("请添加升级材料");
            return;
        }
        int exp = GemModule.Instance.GetExpByStrengthMatList(list);
        if (GTItemHelper.CheckItemEnongh(1, exp) == false)
        {
            return;
        }

        ReqStrengthGem req = new ReqStrengthGem();
        req.TarGem = gem;
        req.UseItems.AddRange(list);
        NetworkManager.Instance.Send<ReqStrengthGem>(MessageID.MSG_REQ_STRENGTHEN_GEM, req, 0, 0);
    }
}
