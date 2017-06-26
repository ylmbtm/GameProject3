using UnityEngine;
using System.Collections;
using Protocol;

public class MountService : GTSingleton<MountService>
{
    public void TryChangeMount(int mountID)
    {
        ReqSetMount req = new ReqSetMount();
        req.MountID = mountID;
        NetworkManager.Instance.Send<ReqSetMount>(MessageID.MSG_REQ_SETMOUNT, req, 0, 0);
    }
}
