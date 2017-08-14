using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using CFG;

namespace MAP
{
    public class FTStep : FTComponent
    {
        [FT] public string        StepName   = string.Empty;
        [FT] public List<FTEvent> StepEvents = new List<FTEvent>();

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
            MapStep data  = new MapStep();
            data.ID       = ID;
            data.StepName = StepName;
            for (int i = 0; i < StepEvents.Count; i++)
            {
                data.StepEvents.Add(StepEvents[i].ID);
            }
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapStep data  = cfg as MapStep;
            this.ID       = data.ID;
            this.StepName = data.StepName;
            FTGroupEvent group = Map.GetGroup<FTGroupEvent>();
            for (int i = 0; i < data.StepEvents.Count; i++)
            {
                FTEvent e = group.GetElement(data.StepEvents[i]);
                if (e != null)
                {
                    StepEvents.Add(e);
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
            this.ID       = UnityEditor.EditorGUILayout.IntField("ID", ID);
            this.StepName = UnityEditor.EditorGUILayout.TextField("StepName", StepName);
#endif
        }
    }
}