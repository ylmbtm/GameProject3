using UnityEngine;
using System.Collections;
using System;
using System.Collections.Generic;
using CFG;

namespace MAP
{
    public class FTWorldBoss : FTComponent
    {
        [FT] public Int32     BossID        = 0;
        [FT] public string    BornBroadcast = string.Empty;
        [FT] public string    OpenBroadcast = string.Empty;
        [FT] public string    DeadBroadcast = string.Empty;
        [FT] public string    FailBroadcast = string.Empty;

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
            MapWorldBoss data  = new MapWorldBoss();
            data.ID            = this.ID;
            data.BossID        = this.BossID;
            data.BornBroadcast = this.BornBroadcast;
            data.OpenBroadcast = this.OpenBroadcast;
            data.DeadBroadcast = this.DeadBroadcast;
            data.FailBroadcast = this.FailBroadcast;
            data.Pos           = this.Pos;
            data.Face          = this.Face;
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapWorldBoss data  = cfg as MapWorldBoss;
            this.ID            = data.ID;
            this.BossID        = data.BossID;
            this.BornBroadcast = data.BornBroadcast;
            this.OpenBroadcast = data.OpenBroadcast;
            this.DeadBroadcast = data.DeadBroadcast;
            this.FailBroadcast = data.FailBroadcast;
            this.Pos           = data.Pos;
            this.Face          = data.Face;
        }

        public override void OnDrawGizmos()
        {
            transform.localScale = new Vector3(10, 20, 10);
            FTDraw.DrawGizmosCylinder(transform.position + new Vector3(0, 40, 0), transform.rotation, transform.localScale, Color.red);
        }

        public override void OnDrawInspector()
        {
#if UNITY_EDITOR
            GUILayout.Space(10);
            this.ID            = UnityEditor.EditorGUILayout.IntField("ID", this.ID);
            this.BossID        = UnityEditor.EditorGUILayout.IntField("BossID", this.BossID);
            this.BornBroadcast = UnityEditor.EditorGUILayout.TextField("BornBroadcast", this.BornBroadcast);
            this.OpenBroadcast = UnityEditor.EditorGUILayout.TextField("OpenBroadcast", this.OpenBroadcast);
            this.DeadBroadcast = UnityEditor.EditorGUILayout.TextField("DeadBroadcast", this.DeadBroadcast);
            this.FailBroadcast = UnityEditor.EditorGUILayout.TextField("FailBroadcast", this.FailBroadcast);
            GUILayout.Space(5);
#endif
        }
    }
}