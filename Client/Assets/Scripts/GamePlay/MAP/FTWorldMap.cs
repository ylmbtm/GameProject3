using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using CFG;

namespace MAP
{
    public class FTWorldMap : MonoBehaviour, IFTWorld
    {
        public int           MapID;
        public int           MapType;
        public string        MapName       = string.Empty;
        public Action        MapLoadFinish = null;
        public MapConfig     MapData       = null;
        public bool          AllowRide     = true;
        public bool          AllowPK       = true;
        public bool          AllowTrade    = true;



        public void AddGroup<T>() where T : FTContainer
        {
            T g = new GameObject(typeof(T).Name).AddComponent<T>();
            g.transform.parent = transform;
            g.transform.localPosition = Vector3.zero;
            g.transform.localEulerAngles = Vector3.zero;
            g.transform.localScale = Vector3.one;
        }

        public T    GetGroup<T>()
        {
            return GetComponentInChildren<T>();
        }

        public void AddGroups()
        {
            AddGroup<FTGroupBorn>();
            AddGroup<FTGroupArea>();
            AddGroup<FTGroupAreaMine>();
            AddGroup<FTGroupAreaMonster>();
            AddGroup<FTGroupBarrier>();
            AddGroup<FTGroupNpc>();
            AddGroup<FTGroupPortal>();
            AddGroup<FTGroupMutiPoint>();
            AddGroup<FTGroupPath>();
            AddGroup<FTGroupBoss>();
            AddGroup<FTGroupEvent>();
            AddGroup<FTGroupTimer>();
            AddGroup<FTGroupStep>();
        }

        public void TriggerElement<T>(int id) where T : FTComponent
        {
            FTGroup<T> g = GetGroup<FTGroup<T>>();
            if (g == null)
            {
                return;
            }
            T element = g.GetElement(id);
            if (element == null)
            {
                return;
            }
            element.Trigger();
        }

        public void ReleaseElement<T>(int id) where T : FTComponent
        {
            FTGroup<T> g = GetGroup<FTGroup<T>>();
            if (g == null)
            {
                return;
            }
            T element = g.GetElement(id);
            if (element == null)
            {
                return;
            }
            element.Release();
        }

        public void EnterWorld(int worldID)
        {
            this.MapID = worldID;
            this.transform.DestroyChildren();
            this.AddGroups();
            this.Import();
            if (MapLoadFinish != null)
            {
                MapLoadFinish();
                MapLoadFinish = null;
            }
        }

        public void ExportDCFGByElement<TElement, TDCFG>(List<TDCFG> cfgList, FTWorldMap worldMap)
        where TElement : FTComponent
        where TDCFG :    MapComponent
        {
            List<TElement> list = worldMap.GetGroup<FTGroup<TElement>>().GetElements();
            for (int i = 0; i < list.Count; i++)
            {
                cfgList.Add(list[i].Export() as TDCFG);
            }
        }

        public void ImportDCFGByElement<TElement, TDCFG>(List<TDCFG> cfgList, FTWorldMap worldMap)
        where TElement : FTComponent
        where TDCFG :    MapComponent
        {
            FTGroup<TElement> group = worldMap.GetGroup<FTGroup<TElement>>();
            for (int i = 0; i < cfgList.Count; i++)
            {
                TElement element = group.AddElement();
                element.Import(cfgList[i]);
            }
        }

        public void ImportEvents(List<MapEvent> cfgList, FTWorldMap worldMap)
        {
            FTGroupEvent group = worldMap.GetGroup<FTGroupEvent>();
            for (int i = 0; i < cfgList.Count; i++)
            {
                MapEvent d     = cfgList[i];
                EFTEventType t = (EFTEventType)d.EventType;
                Type type = Type.GetType("MAP." + t.ToString());
                group.AddEvent(type);
            }
        }

        public void Export()
        {
            FTWorldMap worldMap = this;
            MapConfig  data     = new MapConfig();
            data.MapID          = worldMap.MapID;
            data.MapName        = worldMap.MapName;
            data.MapType        = worldMap.MapType;
            data.AllowPK        = worldMap.AllowPK;
            data.AllowRide      = worldMap.AllowRide;
            data.AllowTrade     = worldMap.AllowTrade;
            ExportDCFGByElement<FTBorn,          MapBorn>(       data.MapBorns,        worldMap);
            ExportDCFGByElement<FTArea,          MapArea>(       data.MapAreas,        worldMap);
            ExportDCFGByElement<FTAreaMine,      MapAreaMine>(   data.MapAreaMines,    worldMap);
            ExportDCFGByElement<FTAreaMonster,   MapAreaMonster>(data.MapAreaMonsters, worldMap);
            ExportDCFGByElement<FTBarrier,       MapBarrier>(    data.MapBarriers,     worldMap);
            ExportDCFGByElement<FTMutiPoint,     MapMutiPoint>(  data.MapMutiPoints,   worldMap);
            ExportDCFGByElement<FTNpc,           MapNpc>(        data.MapNpcs,         worldMap);
            ExportDCFGByElement<FTPath,          MapPath>(       data.MapPaths,        worldMap);
            ExportDCFGByElement<FTPortal,        MapPortal>(     data.MapPortals,      worldMap);
            ExportDCFGByElement<FTStep,          MapStep>(       data.MapSteps,        worldMap);
            ExportDCFGByElement<FTTimer,         MapTimer>(      data.MapTimers,       worldMap);
            ExportDCFGByElement<FTEvent,         MapEvent>(      data.MapEvents,       worldMap);
            string fileName = string.Format("{0}/Resources/Text/Map/{1}.xml", Application.dataPath, data.MapID);
            data.Save(fileName);
        }

        public void Import()
        {
            FTWorldMap worldMap = this;
            string fileName = string.Format("Text/Map/{0}", worldMap.MapID);
            MapConfig data  = new MapConfig();
            data.Load(fileName);
            worldMap.MapID     = data.MapID;
            worldMap.MapName    = data.MapName;
            worldMap.MapType    = data.MapType;
            worldMap.AllowPK    = data.AllowPK;
            worldMap.AllowRide  = data.AllowRide;
            worldMap.AllowTrade = data.AllowTrade;
            ImportEvents(                                        data.MapEvents,       worldMap);
            ImportDCFGByElement<FTBorn,          MapBorn>(       data.MapBorns,        worldMap);
            ImportDCFGByElement<FTArea,          MapArea>(       data.MapAreas,        worldMap);
            ImportDCFGByElement<FTAreaMine,      MapAreaMine>(   data.MapAreaMines,    worldMap);
            ImportDCFGByElement<FTAreaMonster,   MapAreaMonster>(data.MapAreaMonsters, worldMap);       
            ImportDCFGByElement<FTBarrier,       MapBarrier>(    data.MapBarriers,     worldMap);
            ImportDCFGByElement<FTMutiPoint,     MapMutiPoint>(  data.MapMutiPoints,   worldMap);
            ImportDCFGByElement<FTNpc,           MapNpc>(        data.MapNpcs,         worldMap);   
            ImportDCFGByElement<FTPath,          MapPath>(       data.MapPaths,        worldMap);
            ImportDCFGByElement<FTPortal,        MapPortal>(     data.MapPortals,      worldMap);
            ImportDCFGByElement<FTStep,          MapStep>(       data.MapSteps,        worldMap);
            ImportDCFGByElement<FTTimer,         MapTimer>(      data.MapTimers,       worldMap);
            MapData = data;
        }
    }
}