using UnityEngine;
using System.Collections;
using System;
using CFG;

namespace MAP
{
    public class FTNpc : FTComponent
    {
        [FT] public Int32 NpcID = 0;

        public override void Startup()
        {

        }

        public override void Trigger()
        {

        }

        public override void Release()
        {

        }

        public override DCFG Export()
        {
            MapNpc data  = new MapNpc();
            data.ID      = this.ID;
            data.NpcID   = this.NpcID;
            data.Pos     = this.Pos;
            data.Face    = this.Face;
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapNpc data  = cfg as MapNpc;
            this.ID      = data.ID;
            this.NpcID   = data.NpcID;
            this.Pos     = data.Pos;
            this.Face    = data.Face;
        }

        public override void OnDrawGizmos()
        {
            transform.localScale = Vector3.one;
            FTDraw.DrawGizmosCapsule(transform.position + new Vector3(0, 1, 0), transform.rotation, transform.localScale, Color.yellow);
        }

        public override void OnDrawInspector()
        {
#if UNITY_EDITOR
            GUILayout.Space(10);
            this.ID    = UnityEditor.EditorGUILayout.IntField("ID", this.ID);
            this.NpcID = UnityEditor.EditorGUILayout.IntField("NpcID", this.NpcID);
            GUILayout.Space(5);
#endif
        }
    }
}