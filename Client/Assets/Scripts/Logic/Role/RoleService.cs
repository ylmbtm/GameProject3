using UnityEngine;
using System.Collections;
using System;
using Protocol;

public class RoleService : GTSingleton<RoleService>
{
    public void TryAddRoleExp(int exp)
    {
        XCharacter role = RoleModule.Instance.GetCurPlayer();
        int maxLevel = ReadCfgRoleLevel.Count;
        if (role.Level >= maxLevel)
        {
            GTItemHelper.ShowTip("你不能再获得经验了");
            return;
        }
        ReqAddPlayerExp req = new ReqAddPlayerExp();
        req.Exp = exp;
        NetworkManager.Instance.Send(MessageID.MSG_REQ_ADDHERO_EXP, req, 0, 0);
    }
}

