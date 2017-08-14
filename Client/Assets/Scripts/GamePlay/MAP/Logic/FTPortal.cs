using UnityEngine;
using System.Collections;
using CFG;

namespace MAP
{
    public class FTPortal : FTComponent
    {
        [FT] public string                 Name = string.Empty;
        [FT] public int                    DestMapID;
        [FT] public Vector3                DestPos;
        [FT] public EOpenConditionRelation OpenConditionRelation;
        [FT] public int                    OpenLevel;
        [FT] public int                    OpenItemID;
        [FT] public int                    OpenVIP;

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
            MapPortal data             = new MapPortal();
            data.ID                    = this.ID;
            data.Name                  = this.Name;
            data.DestMapID             = this.DestMapID;
            data.DestPos               = this.DestPos;
            data.OpenConditionRelation = (int)OpenConditionRelation;
            data.OpenLevel             = this.OpenLevel;
            data.OpenItemID            = this.OpenItemID;
            data.OpenVIP               = this.OpenVIP;
            data.Pos                   = this.Pos;
            data.Scale                 = this.Scale;
            return data;
        }

        public override void Import(DCFG cfg)
        {
            MapPortal data             = new MapPortal();
            this.ID                    = data.ID;
            this.Name                  = data.Name;
            this.DestMapID             = data.DestMapID;
            this.DestPos               = data.DestPos;
            this.OpenConditionRelation = (EOpenConditionRelation)data.OpenConditionRelation;
            this.OpenLevel             = data.OpenLevel;
            this.OpenItemID            = data.OpenItemID;
            this.OpenVIP               = data.OpenVIP;
            this.Pos                   = data.Pos;
            this.Scale                 = data.Scale;
        }

        public override void OnDrawGizmos()
        {
            float x = Mathf.Clamp(transform.localScale.x, 4f, 2000);
            float z = Mathf.Clamp(transform.localScale.z, 4f, 2000);
            transform.localScale = new Vector3(x, 4, z);
            FTDraw.DrawGizmosCube(transform.position + new Vector3(0, 2, 0), transform.rotation, transform.localScale, new Color32(51, 163, 237, 255), Vector3.one);
        }

        public override void OnDrawInspector()
        {
#if UNITY_EDITOR
            GUILayout.Space(10);
            this.ID                    = UnityEditor.EditorGUILayout.IntField("ID", this.ID);
            this.Name                  = UnityEditor.EditorGUILayout.TextField("Name", this.Name);
            this.DestMapID             = UnityEditor.EditorGUILayout.IntField("DestMapID", this.DestMapID);
            this.DestPos               = UnityEditor.EditorGUILayout.Vector3Field("DestPos", this.DestPos);
            this.OpenConditionRelation = (EOpenConditionRelation)UnityEditor.EditorGUILayout.EnumPopup("OpenConditionRelation", OpenConditionRelation);
            this.OpenLevel             = UnityEditor.EditorGUILayout.IntField("OpenLevel", this.OpenLevel);
            this.OpenItemID            = UnityEditor.EditorGUILayout.IntField("OpenItemID", this.OpenItemID);
            this.OpenVIP               = UnityEditor.EditorGUILayout.IntField("OpenVIP", this.OpenVIP);
            GUILayout.Space(5);
#endif
        }
    }
}