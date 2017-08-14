using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GTDataTimer : GTSingleton<GTDataTimer>
{
    private List<int>    mActionKeyList   = new List<int>();
    private List<long>   mActionNextTimes = new List<long>();
    private long         mGameStartTime;

    public long CurServerTime
    {
        get { return GTTools.GetUtcTime() - mGameStartTime; }
    }

    public long GameStartTime
    {
        get { return mGameStartTime; }
    }

    public override void Init()
    {
        this.LoadSystemInfo();
        this.ActionStart();
    }

    private void LoadSystemInfo()
    {
        mGameStartTime = GTTools.GetUtcTime();
    }

    private void ActionStart()
    {
        mActionKeyList.Clear();
        mActionNextTimes.Clear();
        foreach(var current in ReadCfgAction.Dict)
        {
            mActionKeyList.Add(current.Value.ItemID);
        }
        foreach(var id in mActionKeyList)
        {
            DItem db = ReadCfgItem.GetDataById(id);
            int count = GTDataManager.Instance.GetItemCountById(id);
            long timer = CurServerTime;
            if (count < db.Data1)
            {
                timer += db.Data2;
            }
            mActionNextTimes.Add(timer);
        }
        GTEventCenter.AddHandler(GTEventID.TYPE_SECOND_TICK, ActionUpdate);
    }

    private void ActionExit()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_SECOND_TICK, ActionUpdate);
    }

    private void ActionUpdate()
    {
        for (int i = 0; i < mActionKeyList.Count; i++)
        {
            int key = mActionKeyList[i];
            DItem db = ReadCfgItem.GetDataById(key);
            EAction actionType = GetActionType(key);
            int count =GTDataManager.Instance.GetActionCountByType(actionType);
            if (count >= db.Data1)
            {
                mActionNextTimes[i] = CurServerTime;
            }
            else
            {
                if (CurServerTime >= mActionNextTimes[i])
                {
                    GTDataManager.Instance.AddAction(key, 1);
                    mActionNextTimes[i] = CurServerTime + db.Data2;
                }
            }
        }
    }

    private EAction GetActionType(int key)
    {
        return (EAction)(key - 100);
    }

    public  bool    IsActionFull(EAction type)
    {
        DAction actDB = ReadCfgAction.GetDataById(type);
        DItem db      = ReadCfgItem.GetDataById(actDB.ItemID);
        int count       = GTDataManager.Instance.GetActionCountByType(type);
        return count >= db.Data1;
    }

    public  bool    IsActionFull(int key)
    {
        DItem db = ReadCfgItem.GetDataById(key);
        int count  = GTDataManager.Instance.GetItemCountById(key);
        return count >= db.Data1;
    }

    public  int     GetOneRemainSeconds(EAction actionType)
    {
        int index = 0;
        for (int i = 0; i < mActionKeyList.Count; i++)
        {
            int key = mActionKeyList[i];
            DItem db = ReadCfgItem.GetDataById(key);
            EAction type = GetActionType(key);
            if (type == actionType)
            {
                index = i;
                break;
            }
        }
        return (int)(mActionNextTimes[index] - CurServerTime);
    }

    public  int     GetAllRemainSeconds(EAction actionType)
    {
        int index = 0;
        for (int i = 0; i < mActionKeyList.Count; i++)
        {
            int key = mActionKeyList[i];
            EAction type = GetActionType(key);
            if (type == actionType)
            {
                index = i;
                break;
            }
        }
        int count = GTDataManager.Instance.GetActionCountByType(actionType);
        DAction actDB = ReadCfgAction.GetDataById(actionType);
        DItem db = ReadCfgItem.GetDataById(actDB.ItemID);
        return (int)((db.Data1 - count - 1) * db.Data2 + mActionNextTimes[index] - CurServerTime);
    }

    public  string  GetOneSecondTimer(EAction actionType)
    {
        int sec = this.GetOneRemainSeconds(actionType);
        return GTTools.SecondsToTimer(sec).ToString();
    }

    public  string  GetAllSecondTimer(EAction actionType)
    {
        int sec = this.GetAllRemainSeconds(actionType);
        return GTTools.SecondsToTimer(sec).ToString();
    }
}
