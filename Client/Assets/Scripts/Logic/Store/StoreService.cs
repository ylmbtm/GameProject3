using UnityEngine;
using System.Collections;
using Protocol;

public class StoreService : GTSingleton<StoreService>
{
    public void TryBuyStore(int storeType,int storeID,int num)
    {
        DStore storeDB = ReadCfgStore.GetDataById(storeID);
        if(storeDB==null)
        {
            GTItemHelper.ShowTip("非法物品");
            return;
        }
        if (GTItemHelper.CheckItemEnongh(storeDB.CostMoneyID, storeDB.CostMoneyNum) == false)
        {
            return;
        }
        if (GTItemHelper.CheckBagFull(1))
        {
            return;
        }
        ReqBuyStore req = new ReqBuyStore();
        req.StoreType = storeType;
        req.StoreID = storeID;
        req.Num = num;
        NetworkManager.Instance.Send<ReqBuyStore>(MessageID.MSG_REQ_BUY_STORE, req, 0, 0);
    }
}
