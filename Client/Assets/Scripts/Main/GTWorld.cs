using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using UnityEngine;
using MAP;
using ACT;
using BVT;
using BIE;
using TAS;
using PLT;
using CUT;
using Protocol;
using CFG;

public class GTWorld : GTMonoSingleton<GTWorld>
{
    public EffectSystem           Ect            { get; private set; }
    public FlywordSystem          Fly            { get; private set; }
    public HUDSystem              Hud            { get; private set; }
    public ActSystem              Act            { get; private set; }
    public NodeTreeSystem         Nts            { get; private set; }
    public GuideSystem            Bie            { get; private set; }
    public TaskSystem             Tas            { get; private set; }
    public PlotSystem             Plt            { get; private set; }
    public VideoSystem            Cut            { get; private set; }

    public FTWorldMap             WorldMap       { get; private set; }
    public FTWorldLandscape       WorldLandscape { get; private set; }
    public IEctype                Ectype         { get; private set; }

    public static List<Character> Characters = new List<Character>();
    public static Character       Main { get; set; }
    public static Character       Enem { get; set; }
    public static Character       Boss { get; set; }

    public Character AddActor(int id, EBattleCamp camp, EActorType type, GTTransform bornData, XCharacter data, bool isMain = false)
    {
        Character cc = new Character(id, data == null ? GTGUID.NewGUID() : data.GUID, type, camp, isMain);
        bornData.Pos = GTTools.NavSamplePosition(bornData.Pos);
        cc.Load(bornData);
        cc.SyncData(data, ESyncDataType.TYPE_ALL);
        SetCharacterParent(cc);
        Characters.Add(cc);
        return cc;
    }

    public Character AddActorSync(XCharacter data)
    {
        if (data == null)
        {
            return null;
        }
        GTTransform dTrans = GTTransform.Create(data.PosX, data.PosY, data.PosZ, data.Face);
        Character cc = AddActor(data.Id, (EBattleCamp)data.Camp, (EActorType)data.Type, dTrans, data, false);
        cc.Carrer = data.Carrer;
        cc.IsRemoteActor = true;
        return cc;
    }

    public Character AddActorLocalSync(XCharacter data)
    {
        if (data == null)
        {
            return null;
        }
        GTTransform dTrans = GTTransform.Create(data.PosX, data.PosY, data.PosZ, data.Face);
        Character cc = AddActor(data.Id, (EBattleCamp)data.Camp, (EActorType)data.Type, dTrans, data, false);
        cc.Carrer = data.Carrer;
        cc.IsRemoteActor = true;
        switch(cc.Type)
        {
            case EActorType.MONSTER:
                cc.EnableAI(true);
                cc.OnDeadFinish = () =>
                {
                    if (cc != null && cc.CacheTransform != null)
                    {
                        cc.CacheTransform.gameObject.SetActive(false);
                        GTEventCenter.FireEvent(GTEventID.TYPE_ACTOR_KILL_MONSTER, cc.GUID, cc.ID);
                        cc.OnDeadFinish = null;
                    }
                };
                break;
            case EActorType.NPC:
                cc.EnableAI(true);
                break;
        }
        return cc;
    }

    public Character AddRole(int id, GTTransform bornData)
    {
        return AddActor(id, EBattleCamp.CT_NONE, EActorType.PLAYER, bornData, null, false);
    }

    public Character AddMainPlayer(GTTransform bornData)
    {
        XCharacter data = GTGlobal.Main;
        Main = AddActor(data.Id, EBattleCamp.CT_PVE_PLAYER, EActorType.PLAYER, bornData, data, true);
        Main.Carrer = data.Carrer;
        Main.OnDead = OnDeadForMain;
        return Main;
    }

    public Character AddMainPartner(int pos)
    {
        XCharacter data = null;
        switch (pos)
        {
            case 1:
                data = GTGlobal.MainPartner1;
                break;
            case 2:
                data = GTGlobal.MainPartner2;
                break;
        }
        if (data == null)
        {
            return null;
        }
        Vector3 offset = Vector3.zero;
        switch (pos)
        {
            case 1:
                offset = new Vector3(-2, 0, 0);
                break;
            case 2:
                offset = new Vector3(2, 0, 0);
                break;
        }
        GTTransform bornData = new GTTransform();
        bornData.Euler = Main.Euler;
        bornData.Pos = Main.Pos + offset;
        Character actor = AddActor(data.Id, EBattleCamp.CT_PVE_PLAYER, EActorType.PARTNER, bornData, data);
        actor.Host = Main;
        actor.FollowOffset = offset;
        actor.EnableAI(true);
        switch (pos)
        {
            case 1:
                Main.Partner1 = actor;
                break;
            case 2:
                Main.Partner2 = actor;
                break;
        }
        return actor;
    }

    public Character DelActor(Character cc)
    {
        if (cc == null)
        {
            return null;
        }
        Characters.Remove(cc);
        cc.Release();
        if (cc == Main)
        {
            Main = null;
        }
        return cc;
    }

    public Character DelActor(ulong guid)
    {
        Character cc = GetActor(guid);
        DelActor(cc);
        return null;
    }

    public Character GetActor(ulong guid)
    {
        for (int i = 0; i < Characters.Count; i++)
        {
            if (Characters[i].GUID == guid)
            {
                return Characters[i];
            }
        }
        return null;
    }

    public CharacterAvatar AddAvatar(int modelID)
    {
        DActorModel cfg = ReadCfgActorModel.GetDataById(modelID);
        if (cfg == null)
        {
            return null;
        }
        GameObject obj = GTResourceManager.Instance.Load<GameObject>(cfg.Model, true);
        if (obj == null)
        {
            return null;
        }
        CharacterAvatar avatar = new CharacterAvatar(obj.transform);
        CharacterController cc = obj.GetComponent<CharacterController>();
        if (cc != null)
        {
            cc.enabled = false;
        }
        return avatar;
    }

    public void SyncMainPlayerData(ESyncDataType type)
    {
        if (Main != null)
        {
            GTSimulateServer.CalcMainPropertys();
            Main.SyncData(GTGlobal.Main, type);
        }
    }

    public void SetCharacterParent(Character cc)
    {
        cc.CacheTransform.parent = WorldMap.transform;
    }

    public void EnterWorld(int mapID)
    {
        Ect = new EffectSystem();
        Fly = new FlywordSystem();
        Hud = new HUDSystem();
        Act = new ActSystem();
        Nts = new NodeTreeSystem();
        Plt = new PlotSystem();
        Cut = new VideoSystem();
        WorldMap = new GameObject("FTWorldMap").AddComponent<FTWorldMap>();
        WorldMap.MapLoadFinish = OnLoadMapFinished;
        WorldMap.EnterWorld(mapID);
    }

    public void EnterGuide()
    {
        if (Bie == null)
        {
            Bie = new GuideSystem();
            Bie.Startup();
        }
    }

    public void ResetGuide()
    {
        if (Bie != null)
        {
            Bie.ResetCurGuide();
        }
    }

    public void Release()
    {
        if (Act != null)
        {
            Act.Release();
        }
        if (Hud != null)
        {
            Hud.Release();
        }
        if (Fly != null)
        {
            Fly.Release();
        }
        if (Ect != null)
        {
            Ect.Release();
        }
        if (Nts != null)
        {
            Nts.Release();
        }
        for (int i = 0; i < Characters.Count; i++)
        {
            Character cc = Characters[i];
            cc.Release();
        }
        Characters.Clear();
        Main = null;
        Enem = null;
    }

    public void Execute()
    {
        if (Act != null)
        {
            Act.Execute();
        }
        if (Hud != null)
        {
            Hud.Execute();
        }
        if (Fly != null)
        {
            Fly.Execute();
        }
        if (Nts != null)
        {
            Nts.Execute();
        }
        if (Bie != null)
        {
            Bie.Execute();
        }
    }

    void OnLoadMapFinished()
    {
        OnLoadAvatar();
        OnLoadWindows();
    }

    void OnLoadAvatar()
    {
        if (WorldMap.MapData != null)
        {
            for (int i = 0; i < WorldMap.MapData.MapBorns.Count; i++)
            {
                MapBorn b = WorldMap.MapData.MapBorns[i];
                switch ((EBattleCamp)b.ID)
                {
                    case EBattleCamp.CT_PVE_PLAYER:
                        GTTransform data = GTTransform.Create(b.Pos, b.Face);
                        AddMainPlayer(data);
                        GTCameraManager.Instance.CreateMainCamera(Main.CacheTransform);
                        GTCameraManager.Instance.CreateMiniCamera(Main.CacheFixEuler);
                        GTNetworkSend.Instance.TrySyncIdle();
                        break;
                }
           }
        }
    }

    void OnLoadWindows()
    {
        GTLauncher.Instance.CurScene.OpenWindows();
    }

    void OnDeadForMain(EDeadReason reason)
    {
        GTWindowManager.Instance.OpenWindow(EWindowID.UIReborn);
    }
}