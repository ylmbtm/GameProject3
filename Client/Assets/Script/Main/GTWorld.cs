using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using LVL;
using ACT;
using BVT;

public class GTWorld : GTMonoSingleton<GTWorld>
{
    public EffectSystem   Ect     { get; private set; }
    public FlywordSystem  Fly     { get; private set; }
    public HUDSystem      Hud     { get; private set; }
    public ActSystem      Act     { get; private set; }
    public NodeTreeSystem Nts     { get; private set; }

    public override void SetDontDestroyOnLoad(Transform parent)
    {
        base.SetDontDestroyOnLoad(parent);
        CharacterManager.Instance.SetDontDestroyOnLoad(transform);
        Ect = new EffectSystem();
        Fly = new FlywordSystem();
        Hud = new HUDSystem();
        Act = new ActSystem();
        Nts = new NodeTreeSystem();
    }

    public void EnterWorld(int mapID)
    {
        LevelManager.Instance.EnterWorld(mapID);
    }

    public void Release()
    {
        Act.Release();
        Hud.Release();
        Fly.Release();
        Ect.Release();
        Nts.Release();
        LevelManager.Instance.LeaveWorld();
    }

    void Update()
    {
        Act.Execute();
        Hud.Execute();
        Fly.Execute();
        Nts.Execute();
    }
}