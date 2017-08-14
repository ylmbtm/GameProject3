using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Protocol;
using CFG;
using System;

public class SServer : GTSingleton<SServer>, ICtrl
{
    public SServerCenter SvrCenter = new SServerCenter();
    public SServerWorld  SvrWorld  = new SServerWorld();

    public void AddListener()
    {
        GTEventCenter.AddHandler(GTEventID.TYPE_MAP_ENTERWORLD, OnMapEnterWorld);
        SvrCenter.AddHandlers();
    }

    public void DelListener()
    {
        GTEventCenter.DelHandler(GTEventID.TYPE_MAP_ENTERWORLD, OnMapEnterWorld);
        SvrCenter.DelHandlers();
    }

    private void OnMapEnterWorld()
    {
        SvrWorld.EnterWorld(GTWorld.Instance.WorldMap.MapData);
    }

    private void OnMapLeaveWorld()
    {
        SvrWorld.LeaveWorld();
    }
}
