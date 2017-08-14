using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using CFG;

namespace MAP
{
    public class FTTimer : FTComponent
    {
        [FT] public float         TriggerTime   = 5;
        [FT] public Int32         TriggerLoops  = 1;
        [FT] public List<FTEvent> TriggerEvents = new List<FTEvent>();

        public override void Startup()
        {
            base.Startup();
        }

        public override void Trigger()
        {
            base.Trigger();
        }

        public override void Release()
        {
            base.Release();
        }

        public override DCFG Export()
        {
            MapTimer data = new MapTimer();
            data.ID           = ID;
            data.TriggerTime  = TriggerTime;
            data.TriggerLoops = TriggerLoops;
            for (int i = 0; i < TriggerEvents.Count; i++)
            {
                data.TriggerEvents.Add(TriggerEvents[i].ID);
            }
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapTimer data     = cfg as MapTimer;
            this.ID           = data.ID;
            this.TriggerTime  = data.TriggerTime;
            this.TriggerLoops = data.TriggerLoops;
            FTGroupEvent group = Map.GetGroup<FTGroupEvent>();
            for (int i = 0; i < data.TriggerEvents.Count; i++)
            {
                FTEvent e = group.GetElement(data.TriggerEvents[i]);
                if (e != null)
                {
                    TriggerEvents.Add(e);
                }
            }
        }

        public override void OnDrawGizmos()
        {
            base.OnDrawGizmos();
        }

        public override void OnDrawInspector()
        {
#if UNITY_EDITOR
            this.ID           = UnityEditor.EditorGUILayout.IntField("ID", ID);
            this.TriggerTime  = UnityEditor.EditorGUILayout.FloatField("TriggerTime", TriggerTime);
            this.TriggerLoops = UnityEditor.EditorGUILayout.IntField("TriggerLoops", TriggerLoops);
            FTGUILayout.DrawEvents(Map, TriggerEvents);
#endif
        }
    }
}