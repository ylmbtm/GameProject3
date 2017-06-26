using UnityEngine;
using System.Collections;
using CFG;
using LVL;
using System.Collections.Generic;
using System;

namespace LVL
{
    public class LevelManager : GTMonoSingleton<LevelManager>
    {
        [SerializeField] public int    MapID;
        [SerializeField] public string MapName = string.Empty;

        private MapConfig                            m_Config = new MapConfig();
        private Dictionary<EMapHolder, LevelElement> m_Holders = new Dictionary<EMapHolder, LevelElement>();
        private bool                                 m_IsStart = false;

        public void Init()
        {
            AddHolder<GroupBorn>(EMapHolder.Born);
            AddHolder<GroupMonsterSet>(EMapHolder.MonsterSet);
            AddHolder<GroupWaveSet>(EMapHolder.WaveSet);
            AddHolder<GroupBarrier>(EMapHolder.Barrier);
            AddHolder<GroupRegion>(EMapHolder.Region);
            AddHolder<GroupPortal>(EMapHolder.Portal);
            AddHolder<GroupNpc>(EMapHolder.Npc);
            AddHolder<GroupMineSet>(EMapHolder.MineSet);
            AddHolder<GroupObj>(EMapHolder.Obj);
            foreach (var current in m_Holders)
            {
                current.Value.transform.ResetLocalTransform(transform);
            }
        }

        public void AddHolder<T>(EMapHolder type) where T : LevelElement, ILevelGroup
        {
            LevelElement holder = null;
            m_Holders.TryGetValue(type, out holder);
            if (holder == null)
            {
                holder = new GameObject(typeof(T).Name).AddComponent<T>();
                m_Holders[type] = holder;
            }
        }

        public void EnterWorld(int mapID)
        {
            this.MapID = mapID;
            this.Init();
            if (InitConfig())
            {
                this.OnAwakeSceneEventsStart();
                this.StartCoroutine(DoSceneStartEvents());
            }
            else
            {
                GTLauncher.CurScene.InitWindows();
            }
        }

        public void LeaveWorld()
        {
            m_IsStart = false;
            CharacterCtrl.Instance.DelListener();
            CharacterManager.Instance.DelCharacters();
        }

        public bool IsStart()
        {
            return m_IsStart;
        }

        bool InitConfig()
        {
            string fsPath = LevelUtil.GetConfigPath(this.MapID);
            this.m_Config = new MapConfig();
            return this.m_Config.Load(fsPath);
        }

        void OnAwakeSceneEventsStart()
        {

        }

        IEnumerator AddMainPlayer()
        {
            int id = GTLauncher.CurPlayerID;
            if (m_Config.A == null)
                yield break;
            KTransform bornData = KTransform.Create(m_Config.A.Pos, m_Config.A.Euler);
            CharacterManager.Instance.AddMainPlayer(bornData);
            yield return null;
        }

        IEnumerator AddMainPet()
        {
            yield return null;
        }

        IEnumerator AddPartner()
        {
            CharacterManager.Instance.AddMainPartner(1);
            yield return null;
            CharacterManager.Instance.AddMainPartner(2);
            yield return null;
        }

        IEnumerator AddMonster()
        {
            for (int i = 0; i < m_Config.Monsters.Count; i++)
            {
                MapMonster mm = m_Config.Monsters[i];
                KTransform bd = KTransform.Create(mm.Pos, mm.Euler);
                CharacterManager.Instance.AddMonster(mm.Id, bd);
                yield return null;
            }
        }

        IEnumerator SetFollowCamera()
        {
            GTCameraManager.Instance.CreateMainCamera(CharacterManager.Main.CacheTransform);
            yield return null;
            GTCameraManager.Instance.CreateMinimapCamera(CharacterManager.Main.FixEuler);
        }

        IEnumerator DoSceneStartEvents()
        {
            yield return AddMainPlayer();
            yield return AddMainPet();
            yield return AddPartner();
            yield return AddMonster();
            yield return SetFollowCamera();
            GTLauncher.CurScene.InitWindows();      
            yield return null;
            CharacterCtrl.Instance.DelListener();
            CharacterCtrl.Instance.AddListener();
            m_IsStart = true;
        }
    }
}