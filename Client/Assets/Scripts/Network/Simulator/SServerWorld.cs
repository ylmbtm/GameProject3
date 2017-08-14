using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using Protocol;
using CFG;

public class SServerWorld
{
    private Dictionary<ulong, XCharacter> m_AllActors     = new Dictionary<ulong, XCharacter>();
    private Dictionary<ulong, XCharacter> m_AOIActors     = new Dictionary<ulong, XCharacter>();
    private Dictionary<int,   SGenerator> m_MapGenerators = new Dictionary<int, SGenerator>();

    public void EnterWorld(MapConfig data)
    {
        for (int i = 0; i < data.MapNpcs.Count; i++)
        {
            MapNpc cfg = data.MapNpcs[i];
            DActor db = ReadCfgActor.GetDataById(cfg.NpcID);
            if (db == null)
            {
                continue;
            }
            XCharacter c = new XCharacter();
            c.Id = cfg.NpcID;
            c.Name = db.Name;
            c.GUID = GTGUID.NewGUID();
            c.PosX = cfg.Pos.x;
            c.PosY = cfg.Pos.y;
            c.PosZ = cfg.Pos.z;
            c.Face = cfg.Face;
            c.Type = (int)EActorType.NPC;
            c.Camp = (int)EBattleCamp.CT_PVE_NEUTRAL;
            c.CurAttrs.AddRange(GTTools.GetListFromEnumNames<int>(typeof(EAttr)));
            foreach (var current in db.Attrs)
            {
                c.CurAttrs[(int)current.Key - 1] = current.Value;
            }
            m_AllActors.Add(c.GUID, c);
        }
        for (int i = 0; i < data.MapAreaMonsters.Count; i++)
        {
            MapAreaMonster cfg = data.MapAreaMonsters[i];
            DActor db = ReadCfgActor.GetDataById(cfg.MonsterID);
            if (db == null)
            {
                continue;
            }
            for (int k = 0; k < cfg.Points.Count; k++)
            {
                MapPoint p = cfg.Points[k];
                XCharacter c = new XCharacter();
                c.Id = cfg.MonsterID;
                c.Name = db.Name;
                c.GUID = GTGUID.NewGUID();
                c.PosX = p.Pos.x;
                c.PosY = p.Pos.y;
                c.PosZ = p.Pos.z;
                c.Face = p.EulerAngles.y;
                c.Type = (int)EActorType.MONSTER;
                c.Camp = (int)EBattleCamp.CT_PVE_ENEMY;
                c.CurAttrs.AddRange(GTTools.GetListFromEnumNames<int>(typeof(EAttr)));
                foreach (var current in db.Attrs)
                {
                    c.CurAttrs[(int)current.Key - 1] = current.Value;
                }
                m_AllActors.Add(c.GUID, c);
            }
        }
    }

    public void LeaveWorld()
    {
        m_AllActors.Clear();
        m_AOIActors.Clear();
        m_MapGenerators.Clear();
    }

    public void GetAOI(Vector3 pos)
    {
        List<ulong> delList = new List<ulong>();
        List<XCharacter> addList = new List<XCharacter>();
        HashSet<ulong> allList = new HashSet<ulong>();
        foreach (var current in m_AllActors)
        {
            XCharacter d = current.Value;
            float v = (pos.x - d.PosX) * (pos.x - d.PosX) + (pos.z - d.PosZ) * (pos.z - d.PosZ);
            if (v < 10000)
            {
                allList.Add(d.GUID);
            }
        }
        foreach (var guid in allList)
        {
            if (m_AOIActors.ContainsKey(guid) == false)
            {
                addList.Add(m_AllActors[guid]);
            }
        }
        foreach (var current in m_AOIActors)
        {
            if (allList.Contains(current.Key) == false)
            {
                delList.Add(current.Key);
            }
        }

        if (delList.Count > 0)
        {
            for (int i = 0; i < delList.Count; i++)
            {
                m_AOIActors.Remove(delList[i]);
            }
            LeaveViewNtf ntf = new LeaveViewNtf();
            ntf.List.AddRange(delList);
            NetworkManager.Instance.Send<LeaveViewNtf>(MessageID.MSG_LEAVE_VIEW_NTF, ntf);
        }
        if (addList.Count > 0)
        {
            for (int i = 0; i < addList.Count; i++)
            {
                m_AOIActors.Add(addList[i].GUID, addList[i]);
            }
            EnterViewNtf ntf = new EnterViewNtf();
            ntf.List.AddRange(addList);
            NetworkManager.Instance.Send<EnterViewNtf>(MessageID.MSG_ENTER_VIEW_NTF, ntf);
        }
    }
}